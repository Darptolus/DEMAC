/**
 * @file NMGR.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Manager
 * @todo Add copyright
 *
 * Includes the definition of the Node Manager for each node of the cluster
 */

#include "NMGR.hpp"
#include "Node.hpp"

using namespace decard;

int NMGR::get_tps()
{
  // Assigns TPs to corresponding TPQ
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);

  for (tps_it = all_tps->begin(); tps_it != all_tps->end(); ++tps_it){
    if((*tps_it)->get_dest_id() == n_int->get_id()){
      DECARD_INFOMSG(1, "%s: TP_%05d assigned to INTPQ", n_int->node_name, (*tps_it)->get_id_full());
      (*tps_it)->set_dest(t_node);
      t_INTPQ->push_back(*tps_it);
    }else if((*tps_it)->get_dest_id() != n_int->get_id()){
      DECARD_INFOMSG(1, "%s: TP_%05d assigned to ONTPQ", n_int->node_name, (*tps_it)->get_id_full());
      (*tps_it)->set_dest(nodes_list->at((*tps_it)->get_dest_id()));
      t_ONTPQ->push_back(*tps_it);
    }else{
      DECARD_INFOMSG(1, "%s: TP not assigned", n_int->node_name);
    }
  }
  return 0;
}

int NMGR::all_done(){
  // Check is all Nodes are Done
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  Node_Extern * n_ext;
  int done = 1;
  for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
    if ((*n_it)->get_id() != n_int->node_id){
      n_ext = dynamic_cast <Node_Extern *> (*n_it);
      // Check node status
      if (n_ext->get_mode() != N_DONE){
        done = 0;
      }
    }
  }
  return done;
}

int NMGR::run()
{
  // Initialize NMGR 
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  Node_Extern * n_ext;
  ThreadedProcedure * newTP;
  tp_type tptype;
  Message * newMsg;
  ops_type op_type;
  int done;

  DECARD_INFOMSG(1, "%s: NMGR: INIT", n_int->node_name);
  do{
    switch(this->get_mode()) {
      case M_DONE: // Done Mode
      DECARD_INFOMSG(1, "%s: NMGR: DONE", n_int->node_name);   
        // Scheduling queues are empty -> Set Node Mode DONE 
        DECARD_INFOMSG(1, "%s: NMGR: ALL DONE", n_int->node_name);
        n_int->clr_exec();
        // Change to IDLE
        // this->mode_idl();
      break; // End Done Mode

      case M_IDLE: // Idle Mode
        DECARD_INFOMSG(1, "%s: NMGR: IDLE INC=%03d ONC=%03d INT=%03d ONT=%03d", n_int->node_name, 
        t_INCLQ->size(), t_ONCLQ->size(), t_INTPQ->size(), t_ONTPQ->size());
        // Check if all Queues are empty
        if (!t_INTPQ->empty() || !t_INCLQ->empty()){
          // TODO: Check all SU
          if (t_SU->is_full()){
            // OSTPQ > Max Local TP -> Change to REMOTE
            this->mode_rmt();
          }else{
            // INTPQ > 0 & SU available -> Change to LOCAL
            this->mode_lcl();
          }
        }else if (this->all_empty()){
          // Queues Empty
          DECARD_INFOMSG(1, "%s: NMGR: IDLE EMPTY", n_int->node_name);
          if (n_int->get_mode() == N_DONE && this->all_done() && n_int->get_cidle()){
            // Node Done, All Nodes are DONE, NCOM Idle
            this->mode_dne();
          }else{
          // } else if (this->get_mode() == M_IDLE){
            // Stay in IDLE
            usleep(1000000);
          }
        }
        if (this->get_mode() == M_IDLE){
          // INVALID
          DECARD_INFOMSG(1, "%s: NMGR: IDLE", n_int->node_name);
          usleep(1000000);
        }
      break; // End Idle Mode

      case M_LOCL: // Local Mode
      // DECARD_INFOMSG(1, "%s: NMGR: LCAL", n_int->node_name);
      // Check queues
      if (!t_INTPQ->empty()){
        newTP = t_INTPQ->popFront();
        tptype = newTP->get_tptype();
        DECARD_INFOMSG(1, "%s: NMGR: LCAL NO_%03d TP_%04d", n_int->node_name, newTP->get_orig_id(), newTP->get_id_full());
        if(tptype == END){
          // End TP -> Send Done Message to NCOM
          DECARD_INFOMSG(1, "%s: NMGR: LCAL END_TP", n_int->node_name);
          n_int->mode_dne();
          op_type = N_D;
          newMsg = new Message(op_type, this->t_node);
          t_ONCLQ->push_back(newMsg);
        }else{
          DECARD_INFOMSG(1, "%s: NMGR: LCAL Assign to SU", n_int->node_name);
          // Check for available ISTPQ
          // ToDo: add multiple ISTPQs
            // if (t_ISTPQ->size() < max_istpq){
            if (t_SU->is_avail()){
              // Assign TP to available ISTPQ
              // t_ISTPQ->push_back(newTP);
              t_SU->add_TP(newTP);
            }
        }
      }else if (!t_INCLQ->empty()){
        newMsg = t_INCLQ->popFront();
        op_type = newMsg->get_opr();
        DECARD_INFOMSG(1, "%s: NMGR: LCAL RO_%03d TP_%04d", n_int->node_name, newMsg->get_orig_id(), newMsg->get_opr());
        // if(op_type == N_D){ // Node Done
        //   this->mode_dne();
        // }
      }
      // Check conditions for next state
      if (t_INTPQ->empty() && t_INCLQ->empty()){
        this->mode_idl();
      }
      break; // End Local Mode

    case M_REMT: // Remote Mode
      DECARD_INFOMSG(1, "%s: NMGR: REMT", n_int->node_name);
      // Max_LTP reached
      
      // Change to IDLE
      this->mode_idl();
      break; // End Remote Mode

    default:
      DECARD_INFOMSG(1, "%s: NMGR: Invalid State", n_int->node_name);

    } // End switch

  }while(n_int->get_exec());
  return 0;
}

// int NMGR::tst_gen_0()
// {
//   Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
//   ThreadedProcedure * newTP;
//   int x, y;
//   // Producing
//   DECARD_INFOMSG(1, "%s: NMGR: Generating TPs", n_int->node_name);
//   for (x = 0; x < 3; ++x){
//     newTP = new ThreadedProcedure();
//     newTP->set_orig_id(n_int->node_id);
//     // newTP->set_dest((*n_it)->get_id());
//     newTP->set_opr(x);
//     DECARD_INFOMSG(1, "%s: NMGR: Pushing [%d]", n_int->node_name, x);
//     t_ONTPQ->push_back(newTP);
//     // t_OCTRQ->push_back(x);
//   }
//   usleep(1000000);
//   // Consuming
//   DECARD_INFOMSG(1, "%s: NMGR: Consuming TPs", n_int->node_name);
//   x = 0;
//   while (x < 3) {
//     if(!t_INTPQ->empty()){
//       newTP = t_INTPQ->popFront();
//       y = *(newTP->get_opr());
//       DECARD_INFOMSG(1, "%s: NMGR: Popping [%d]", n_int->node_name, y);
//       ++x;
//     }
//   }
//   return 0;
// }

// TP Generation test
// int NMGR::tst_gen_1(int y)
// {
//   Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
//   Node_Extern * n_ext;
//   ThreadedProcedure * newTP;
//   int x, tp_id;
//   int done;
//   // Producing
//   DECARD_INFOMSG(1, "%s: NMGR: Test_Gen TPs", n_int->node_name);

//   do{
//     switch(this->get_mode()) {
//     case M_DONE: // Done Mode
//       DECARD_INFOMSG(1, "%s: NMGR: DONE", n_int->node_name);
//       // Check if all nodes are DONE
//       done = 1;
//       for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
//         if ((*n_it)->get_id() != n_int->node_id){
//           n_ext = dynamic_cast <Node_Extern *> (*n_it);
//           // Check node status
//           if (n_ext->get_mode() != N_DONE){
//             done = 0;
//           }
//         }
//       }
//       if(done && t_OSTPQ->empty() && t_ISTPQ->empty()){
//         // ToDo: add INTPQ & ONTPQ
//         // Scheduling queues are empty -> Set Node Mode DONE 
//         n_int->mode_dne();
//         usleep(1000000);
//       }else {
//         // Change to IDLE
//         this->mode_idl();
//       }
//       break;

//     case M_IDLE: // Idle Mode
//       DECARD_INFOMSG(1, "%s: NMGR: IDLE", n_int->node_name);
//       if (t_OSTPQ->size() > get_mx_ostpq()){
//         // OSTPQ > MAX Local TP -> Change to REMOTE
//         this->mode_rmt();
//       } else if (!t_INTPQ->empty()){
//         // INTPQ > 0 -> Change to LOCAL
//         this->mode_lcl();
//       } else if (this->get_mode() == M_IDLE){
//         // Stay in IDLE
//         usleep(1000000);
//       } else{
//         // INVALID
//         DECARD_INFOMSG(1, "%s: NMGR: Invalid State", n_int->node_name);
//         usleep(1000000);
//       }
//       break;

//     case M_LOCL: // Local Mode
//       DECARD_INFOMSG(1, "%s: NMGR: LCAL", n_int->node_name);
//       // Check for available SU

//       // Simulate Consuming -> Assigning to SU
//       if (!t_INTPQ->empty()){
//         newTP = t_INTPQ->popFront();
//         tp_id = *(newTP->get_opr());
//         DECARD_INFOMSG(1, "%s: NMGR: LCAL RO_%03d TP_%04d", n_int->node_name, newTP->get_orig_id(), tp_id);
//       }

//       if(tp_id == 999){
//         // "DONE" Message
//         // n_it = nodes_list->at(newTP->get_orig());
//         for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
//           if ((*n_it)->get_id() == newTP->get_orig_id()){
//             n_ext = dynamic_cast <Node_Extern *> (*n_it);
//             n_ext->mode_dne();
//             this->mode_dne();
//           }
//         }
//       }else{
//         this->mode_idl();
//       }
      
//       break;

//     case M_REMT: // Remote Mode
//       DECARD_INFOMSG(1, "%s: NMGR: REMT", n_int->node_name);
//       // Simulate generation -> max_LTP reached
//       x=0;
//       do{
//         for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
//           // printf("%s: NMGR x= %d, y= %d\n", n_int->node_name,x,y);
//           if ((*n_it)->get_id() != n_int->node_id){
//             n_ext = dynamic_cast <Node_Extern *> (*n_it);
//             if (x==y){
//               tp_id = 999;
//             }else{
//               tp_id = (n_ext->get_id() + 1) * 1000 + (n_int->node_id + 1) * 10 + x;
//             }      
//             // Generate TP
//             newTP = new ThreadedProcedure();
//             newTP->set_orig_id(n_int->node_id);
//             newTP->set_dest_id(n_ext->get_id());
//             newTP->set_opr(tp_id);
//             DECARD_INFOMSG(1, "%s: NMGR: REMT RD_%03d TP_%04d", n_int->node_name, n_ext->get_id(), tp_id);
//             t_ONTPQ->push_back(newTP);
//           }
//         }
//         ++x;
//       }while(x <= y);
//       // Change to IDLE
//       this->mode_idl();
//       break;

//     default:
//       DECARD_INFOMSG(1, "%s: NMGR: Invalid State", n_int->node_name);
//     }
//   }while(n_int->get_exec());

//   return 0;
// }
