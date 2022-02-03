/**
 * @file NCOM.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Comunicator
 * @todo Add copyright
 *
 * Includes the definition of the Node Comunicator for each node of the cluster
 */

#include "NCOM.hpp"
#include "Node.hpp"

using namespace decard;

int NCOM::run()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  // Node_Intern * n_rcv = dynamic_cast <Node_Intern *> (n_recv);
  // todo: t_node -> n_int
  Node_Extern * n_ext;
  ThreadedProcedure * newTP;
  tp_type tptype;
  Message * newMsg;
  ops_type op_type;
  // NCOM initialize channels
  DECARD_INFOMSG(1, "%s: NCOM: INIT", n_int->node_name);
  // DECARD_INFOMSG(1, "This is a test");
  do{
    switch(this->get_mode()) {
    case C_IDLE: // Idle Mode
      DECARD_INFOMSG(1, "%s: NCOM: IDLE", n_int->node_name);
      // Check Inbox for each Node
      for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
        if ((*n_it)->get_id() != n_int->node_id){
          n_ext = dynamic_cast <Node_Extern *> (*n_it);
          // Check if msg has been recieved - Non Blocking
          ErrCode = MPI_Test(n_ext->get_rreq(), n_ext->get_rflg(), n_ext->get_rsts());
          // Check if msg has been recieved - Blocking
          // ErrCode = MPI_Wait(n_ext->get_rreq(), n_ext->get_rsts());
          // Set next state 
          if (*(n_ext->get_rflg())){
            if(!ErrCode){
              DECARD_INFOMSG(1, "%s: NCOM: TEST RO_%03d M_%04d S_%d", 
              n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox()), *(n_ext->get_rflg()));
            }else{
              DECARD_INFOMSG(1, "%s: NCOM: TEST RO_%03d M_%04d S_%d E_%d", 
              n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox()), *(n_ext->get_rflg()), ErrCode);
            }
            // Mssage recieved -> Set origin node -> Change to RECEIVE
            n_int->set_nrcv(n_ext);
            n_ext->clr_renb();
            this->mode_rcv();
            n_int->clr_cidle(); // Communicator Not Idle
            break;
          } else if ((!t_ONTPQ->empty()) || (!t_ONCLQ->empty())){
            // Outgoing message(s) -> Change to SEND
            this->mode_snd();
            n_int->clr_cidle(); // Communicator Not Idle
            break;
          }
        }
      }
      if (this->get_mode() == C_IDLE){
        // Stay in IDLE
        n_int->set_cidle();
        usleep(1000000);
      }
    break; // End Idle Mode

    case C_RECV: // Recieve Mode
      DECARD_INFOMSG(1, "%s: NCOM: RECV", n_int->node_name);
      // Check for incoming message
      if (n_int->get_nrcv()){
        // Message Recieved
        DECARD_INFOMSG(1, "%s: NCOM: RECV RO_%03d M_%04d", n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox())); // *((n_int->get_nrcv())->get_msgbox())
        // Check if Node Done
        if (*(n_ext->get_msgbox()) == 999){
          n_ext->mode_dne(); // Set External node status to Done
          // Generate control Message 
          op_type = N_D;
          newMsg = new Message(op_type, n_ext);
          t_INCLQ->push_back(newMsg);
        }else{
          // Push to queue
          newTP = new ThreadedProcedure();
          // Simulated info from origin
          newTP->set_orig(n_ext);
          newTP->set_orig_id(n_ext->get_id());
          newTP->set_dest(n_int);
          newTP->set_dest_id(n_int->node_id);
          newTP->set_opr(*(n_ext->get_msgbox()));
          // Push to INTPQ
          t_INTPQ->push_back(newTP);
        }
        // Clear Recieved
        n_int->set_nrcv(NULL);
        // Reenable Channel
        ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
        n_ext->set_renb();
        if(!ErrCode){
          DECARD_INFOMSG(1, "%s: NCOM: OPEN RD_%03d", n_int->node_name, n_ext->get_id());
        }else{
          DECARD_INFOMSG(1, "%s: NCOM: OPEN RD_%03d E_%d", n_int->node_name, n_ext->get_id(), ErrCode);
        }
      } else {
        // Check all channels
        for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
          // Check channel availability
          if ((*n_it)->get_id() != n_int->node_id){
            n_ext = dynamic_cast <Node_Extern *> (*n_it);
            if (!(n_ext->get_renb())){
              // Open channel
              ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
              n_ext->set_renb();
              if(!ErrCode){
                DECARD_INFOMSG(1, "%s: NCOM: OPEN RD_%03d", n_int->node_name, n_ext->get_id());
              }else{
                DECARD_INFOMSG(1, "%s: NCOM: OPEN RD_%03d E_%d", n_int->node_name, n_ext->get_id(), ErrCode);
              }
            }
          }
        }
      }

      if ((!t_ONTPQ->empty()) || (!t_ONTPQ->empty())){
        // Outgoing message(s) -> Change to SEND
        this->mode_snd();
      } else {
        // Change to IDLE
        this->mode_idl();
      }
    break; // End Receive Mode

    case C_SEND:
    DECARD_INFOMSG(1, "%s: NCOM: SEND", n_int->node_name);
      if(!t_ONTPQ->empty()){
        // Select Receiving Node
        // Send msg to node
        newTP = t_ONTPQ->popFront();
        // ACK before sending to the same node twice?
        ErrCode = MPI_Send(newTP->get_opr(), 1, MPI_INT, newTP->get_dest_id(), 1, MPI_COMM_WORLD);
        if(!ErrCode){
          DECARD_INFOMSG(1, "%s: NCOM: SEND RD_%03d M_%04d", 
          n_int->node_name, newTP->get_dest_id(), *(newTP->get_opr()));
        }else{
          DECARD_INFOMSG(1, "%s: NCOM: SEND RD_%03d M_%04d E_%d", 
          n_int->node_name, newTP->get_dest_id(), *(newTP->get_opr()), ErrCode);
        }
      }else if(!t_ONCLQ->empty()){
        // Outgoing Control Message
        newMsg = t_ONCLQ->popFront();
        switch (newMsg->get_opr()){
          // Node status - Broadcast
          case N_S: // Node Sending
            // High Workload -> Switch to Send Mode
            DECARD_INFOMSG(1, "%s: NCOM: SEND N_S", n_int->node_name);
          break;
          case N_R: // Node Receiving
            // Low Workload -> Switch to Receive Mode
            DECARD_INFOMSG(1, "%s: NCOM: SEND  N_R", n_int->node_name);
          break;
          case N_D: // Node Done
            DECARD_INFOMSG(1, "%s: NCOM: SEND N_D", n_int->node_name);
            // Check all channels
            for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
              // Check channel availability
              if ((*n_it)->get_id() != n_int->node_id){
                int oopr = 999;
                ErrCode = MPI_Send(&oopr, 1, MPI_INT, (*n_it)->get_id(), 1, MPI_COMM_WORLD);
                if(!ErrCode){
                  DECARD_INFOMSG(1, "%s: NCOM: SEND RD_%03d M_%04d", 
                  n_int->node_name, (*n_it)->get_id(), oopr);
                }else{
                  DECARD_INFOMSG(1, "%s: NCOM: SEND RD_%03d M_%04d E_%d", 
                  n_int->node_name, (*n_it)->get_id(), oopr, ErrCode);
                }
              }
            }    
          break; // Node Done

          case N_E: // Node Error
            DECARD_INFOMSG(1, "%s: NCOM: SEND N_E", n_int->node_name);
          break;
          // Workload Management - P2P
          case TSR: // TP Send Request
            DECARD_INFOMSG(1, "%s: NCOM: SEND TSR", n_int->node_name);
          break;
          case TRR: // TP Receive Request
            DECARD_INFOMSG(1, "%s: NCOM: SEND TRR", n_int->node_name);
          break;
          case TSE: // TP Send
            DECARD_INFOMSG(1, "%s: NCOM: SEND TSE", n_int->node_name);
          break;
          case TRE: // TP Receive
            DECARD_INFOMSG(1, "%s: NCOM: SEND TRE", n_int->node_name);
          break;
          case TDN: // TP Done
            DECARD_INFOMSG(1, "%s: NCOM: SEND TDN", n_int->node_name);
          break;
          // Data Management - P2P
          case DSR: // Data Send Request
            DECARD_INFOMSG(1, "%s: NCOM: SEND DSR", n_int->node_name);
          break;
          case DRR: // Data Receive Request
            DECARD_INFOMSG(1, "%s: NCOM: SEND DRR", n_int->node_name);
          break;
          case DSE: // Data Send Request
            DECARD_INFOMSG(1, "%s: NCOM: SEND DSE", n_int->node_name);
          break;
          case DRE: // Data Receive Request
            DECARD_INFOMSG(1, "%s: NCOM: SEND DRE", n_int->node_name);
          break;
          default:
            DECARD_INFOMSG(1, "%s: NCOM: SEND Default", n_int->node_name);
        }
      }
      // Check if there are still outgoing messages
      if ((!t_ONTPQ->empty()) || (!t_ONCLQ->empty())){
        // Outgoing message(s) -> Change to SEND
        this->mode_snd();
      }else{
        this->mode_idl();
      }
      // while (!t_ONTPQ->empty()){
      //   // Send msg to node
      //   newTP = t_ONTPQ->popFront();
      //   // ACK before sending to the same node twice?
      //   ErrCode = MPI_Send(newTP->get_opr(), 1, MPI_INT, newTP->get_dest_id(), 1, MPI_COMM_WORLD);
      //   if(!ErrCode){
      //     DECARD_INFOMSG(1, "%s: NCOM: SEND RD_%03d M_%04d", 
      //     n_int->node_name, newTP->get_dest_id(), *(newTP->get_opr()));
      //   }else{
      //     DECARD_INFOMSG(1, "%s: NCOM: SEND RD_%03d M_%04d E_%d", 
      //     n_int->node_name, newTP->get_dest_id(), *(newTP->get_opr()), ErrCode);
      //   }
      // }   
      // Change to IDLE

    break; // End Send Mode

    default:
      DECARD_INFOMSG(1, "%s: NCOM: Invalid State", n_int->node_name);

    } // End Switch
  }while (n_int->get_exec());
  return 0;
}

// int NCOM::tst_gen_0()
// // TOBEDELETED
// {
//   Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
//   ThreadedProcedure * newTP;
//   // Consuming
//   DECARD_INFOMSG(1, "%s: NCOM: Consuming TPs", n_int->node_name);
//   int x, y;
//   x = 0;
//   while (x < 3) {
//     if (!t_ONTPQ->empty()){
//       newTP = t_ONTPQ->popFront();
//       y = *(newTP->get_opr());
//       DECARD_INFOMSG(1, "%s: NCOM: Popping [%d]", n_int->node_name, y);
//       ++x;
//     }
//   }
//   usleep(1000000);
//   // Producing
//   DECARD_INFOMSG(1, "%s: NCOM: Generating TPs", n_int->node_name);
//   for (x = 0; x < 3; ++x){
//     newTP = new ThreadedProcedure();
//     newTP->set_orig_id(n_int->node_id);
//     // newTP->set_dest((*n_it)->get_id());
//     newTP->set_opr(x);
//     DECARD_INFOMSG(1, "%s: NCOM: Pushing [%d]", n_int->node_name, x);
//     t_INTPQ->push_back(newTP);
//     // this_OCTRQ->push_back(x);
//   }
//   return 0;
// }

// int NCOM::tst_gen_1()
// {
//   Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
//   // Node_Intern * n_rcv = dynamic_cast <Node_Intern *> (n_recv);
//   // todo: t_node -> n_int
//   Node_Extern * n_ext;
//   ThreadedProcedure * newTP;

//   // NCOM initialize channels
//   DECARD_INFOMSG(1, "%s: NCOM: INIT", n_int->node_name);
//   // DECARD_INFOMSG(1, "This is a test");
//   do{
//     switch(this->get_mode()) {
//     // case N_DONE: // Done Mode
//     //   DECARD_INFOMSG(1, "%s: NCOM: DONE", n_int->node_name);
//     //   // Send DONE msg

//     //   // Wait for ACK?

//     //   // set exec = 0
//     //   n_int->clr_exec();
//     //   break;

//     case C_IDLE: // Idle Mode
//       DECARD_INFOMSG(1, "%s: NCOM: IDLE", n_int->node_name);
//       // Test
//       for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
//         if ((*n_it)->get_id() != n_int->node_id){
//           n_ext = dynamic_cast <Node_Extern *> (*n_it);
//           // Check if msg has been recieved

//           // ErrCode = MPI_Wait(n_ext->get_rreq(), n_ext->get_rsts());
//           ErrCode = MPI_Test(n_ext->get_rreq(), n_ext->get_rflg(), n_ext->get_rsts());

//           if(!ErrCode){
//             DECARD_INFOMSG(1, "%s: NCOM: TEST RO_%03d M_%04d S_%d", 
//             n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox()), *(n_ext->get_rflg()));
//           }else{
//             DECARD_INFOMSG(1, "%s: NCOM: TEST RO_%03d M_%04d S_%d E_%d", 
//             n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox()), *(n_ext->get_rflg()), ErrCode);
//           }

//           if (*(n_ext->get_rflg())){
//             // Mssage recieved -> Set origin node -> Change to RECEIVE
//             n_int->set_nrcv(n_ext);
//             n_ext->clr_renb();
//             this->mode_rcv();
//             n_int->clr_cidle(); // Communicator Not Idle
//             break;
//           } else if ((!t_ONTPQ->empty()) || (!t_ONTPQ->empty())){
//             // Outgoing message(s) -> Change to SEND
//             this->mode_snd();
//             n_int->clr_cidle(); // Communicator Not Idle
//             break;
//           }
//         }
//       }
//       if (this->get_mode() == C_IDLE){
//         // Stay in IDLE
//         n_int->set_cidle();
//         usleep(1000000);
//       }
//       break;

//     case C_RECV: // Recieve Mode
//       DECARD_INFOMSG(1, "%s: NCOM: RECV", n_int->node_name);
//       // Check for incoming message
//       if (n_int->get_nrcv()){
//         // Message Recieved
//         DECARD_INFOMSG(1, "%s: NCOM: RECV RO_%03d M_%04d", n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox())); // *((n_int->get_nrcv())->get_msgbox())
//         // Push to queue
//         newTP = new ThreadedProcedure();
//         // Simulated info from origin
//         newTP->set_orig_id(n_ext->get_id());
//         newTP->set_dest_id(n_int->node_id);
//         newTP->set_opr(*(n_ext->get_msgbox()));
//         // [END] Simulated info from origin
//         t_INTPQ->push_back(newTP);
//         // Clear Recieved
//         n_int->set_nrcv(NULL);
//         // Reenable Channel
//         ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
//         n_ext->set_renb();
//         if(!ErrCode){
//           DECARD_INFOMSG(1, "%s: NCOM: OPEN RD_%03d", n_int->node_name, n_ext->get_id());
//         }else{
//           DECARD_INFOMSG(1, "%s: NCOM: OPEN RD_%03d E_%d", n_int->node_name, n_ext->get_id(), ErrCode);
//         }
//       } else {
//         // Check all channels
//         for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
//           // Check channel availability
//           if ((*n_it)->get_id() != n_int->node_id){
//             n_ext = dynamic_cast <Node_Extern *> (*n_it);
//             if (!(n_ext->get_renb())){
//               // Open channel
//               ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
//               n_ext->set_renb();
//               if(!ErrCode){
//                 DECARD_INFOMSG(1, "%s: NCOM: OPEN RD_%03d", n_int->node_name, n_ext->get_id());
//               }else{
//                 DECARD_INFOMSG(1, "%s: NCOM: OPEN RD_%03d E_%d", n_int->node_name, n_ext->get_id(), ErrCode);
//               }
//             }
//           }
//         }
//       }

//       if ((!t_ONTPQ->empty()) || (!t_ONTPQ->empty())){
//         // Outgoing message(s) -> Change to SEND
//         this->mode_snd();
//       } else {
//         // Change to IDLE
//         this->mode_idl();
//       }
//       break;

//     case C_SEND:
//     DECARD_INFOMSG(1, "%s: NCOM: SEND", n_int->node_name);
//       while (!t_ONTPQ->empty()){
//         // Send msg to node
//         newTP = t_ONTPQ->popFront();
//         // ACK before sending to the same node twice?
//         ErrCode = MPI_Send(newTP->get_opr(), 1, MPI_INT, newTP->get_dest_id(), 1, MPI_COMM_WORLD);
//         if(!ErrCode){
//           DECARD_INFOMSG(1, "%s: NCOM: SEND RD_%03d M_%04d", 
//           n_int->node_name, newTP->get_dest_id(), *(newTP->get_opr()));
//         }else{
//           DECARD_INFOMSG(1, "%s: NCOM: SEND RD_%03d M_%04d E_%d", 
//           n_int->node_name, newTP->get_dest_id(), *(newTP->get_opr()), ErrCode);
//         }
//       }   
//       // Change to IDLE
//       this->mode_idl();

//       break;
//     default:
//       DECARD_INFOMSG(1, "%s: NCOM: Invalid State", n_int->node_name);
//     }
//   }while (n_int->get_exec());

//   return 0;
// }
    // case N_DONE: // Done Mode
    //   DECARD_INFOMSG(1, "%s: NCOM: DONE", n_int->node_name);
    //   // Send DONE msg

    //   // Wait for ACK?

    //   // set exec = 0
    //   n_int->clr_exec();
    //   break;