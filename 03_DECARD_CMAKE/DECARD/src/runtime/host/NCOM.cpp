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
  // int codeA = 0;
  // int codeB = 0;
  // int node_dest = 0;
  // int MsgA = 10;
  // int MsgB = 0;
  // int* MsgDest;
  // int ErrCode = 0;

  // int test_flagS = 0;
  // int test_flagR = 0;

  // NCOM initialize channels
  printf("%s: NCOM: INIT\n", n_int->node_name);
  
  // for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
  //   // Clear channel availability
  //   n_ext = dynamic_cast <Node_Extern *> (*n_it);
  //   n_ext->clr_renb();
  // }

  do{
    switch(n_int->get_mode()) {
    case N_DONE: // Done Mode
      printf("%s: NCOM: DONE\n", n_int->node_name);
      // Send DONE msg

      // Wait for ACK?

      // set exec = 0
      n_int->clr_exec();
      break;

    case N_IDLE: // Idle Mode
      printf("%s: NCOM: IDLE\n", n_int->node_name);
      // Test
      for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
        if ((*n_it)->get_id() != n_int->node_id){
          n_ext = dynamic_cast <Node_Extern *> (*n_it);
          // Check if msg has been recieved

          // ErrCode = MPI_Wait(n_ext->get_rreq(), n_ext->get_rsts());
          ErrCode = MPI_Test(n_ext->get_rreq(), n_ext->get_rflg(), n_ext->get_rsts());

          if(!ErrCode){
            printf("%s: NCOM: TEST RO_%03d M_%04d S_%d\n", 
            n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox()), *(n_ext->get_rflg()));
          }else{
            printf("%s: NCOM: TEST RO_%03d M_%04d S_%d E_%d\n", 
            n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox()), *(n_ext->get_rflg()), ErrCode);
          }

          // printf("%s: NCOM: TEST R[%d] msg(%d) \t from R(%d/%d) \t to R(%d) \t ErrCode(%d)\n", 
          // n_int->node_name, *(n_ext->get_rflg()), *(n_ext->get_msgbox()), n_int->node_id, n_int->world_size, n_ext->get_id(), ErrCode);
          if (*(n_ext->get_rflg())){
            // Mssage recieved -> Set origin node -> Change to RECEIVE
            n_int->set_nrcv(n_ext);
            n_ext->clr_renb();
            n_int->mode_rcv();
            break;
          } else if ((!t_ONTPQ->empty()) || (!t_OCTRQ->empty())){
            // Outgoing message(s) -> Change to SEND
            n_int->mode_snd();
            break;
          }
        }
      }
      if (n_int->get_mode() == N_IDLE){
        // Stay in IDLE
        usleep(1000000);
      }
      break;

    case N_RECEIVE: // Recieve Mode
      printf("%s: NCOM: RECV\n", n_int->node_name);
      // Check for incoming message
      // printf("%s: NCOM nrcv = %p\n", n_int->node_name, n_int->get_nrcv());
      if (n_int->get_nrcv()){
        // Message Recieved
        printf("%s: NCOM: RECV RO_%03d M_%04d\n", n_int->node_name, n_ext->get_id(), *(n_ext->get_msgbox())); // *((n_int->get_nrcv())->get_msgbox())
        
        // printf("%s: NCOM: Pushing [%d] to INTPQ from rank %d\n", n_int->node_name, *(n_ext->get_msgbox()), n_ext->get_id()); // *((n_int->get_nrcv())->get_msgbox())
        // Push to queue
        newTP = new ThreadedProcedure();
        // Simulated info from origin
        newTP->set_orig(n_ext->get_id());
        newTP->set_dest(n_int->node_id);
        newTP->set_opr(*(n_ext->get_msgbox()));
        // [END] Simulated info from origin
        t_INTPQ->push_back(newTP);
        // Clear Recieved
        n_int->set_nrcv(NULL);
        // Reenable Channel
        ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
        n_ext->set_renb();
        if(!ErrCode){
          printf("%s: NCOM: OPEN RD_%03d\n", n_int->node_name, n_ext->get_id());
        }else{
          printf("%s: NCOM: OPEN RD_%03d E_%d\n", n_int->node_name, n_ext->get_id(), ErrCode);
        }
        

        // printf("%s: NCOM: OPEN OR(%3d/%3d) DR(%d) EC(%d)\n",
        // n_int->node_name, n_int->node_id, n_int->world_size, n_ext->get_id(), ErrCode);
      } else {
        // Check all channels
        for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
          // Check channel availability
          // printf("%s: NCOM Here RA(%d) RB(%d)\n", n_int->node_name, n_int->node_id, (*n_it)->get_id());
          if ((*n_it)->get_id() != n_int->node_id){
            n_ext = dynamic_cast <Node_Extern *> (*n_it);
            if (!(n_ext->get_renb())){
              // Open channel
              ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
              n_ext->set_renb();
              if(!ErrCode){
                printf("%s: NCOM: OPEN RD_%03d\n", n_int->node_name, n_ext->get_id());
              }else{
                printf("%s: NCOM: OPEN RD_%03d E_%d\n", n_int->node_name, n_ext->get_id(), ErrCode);
              }
            }
          }
        }
      }

      if ((!t_ONTPQ->empty()) || (!t_OCTRQ->empty())){
        // Outgoing message(s) -> Change to SEND
        n_int->mode_snd();
      } else {
        // Change to IDLE
        n_int->mode_idl();
      }
      break;

    case N_SEND:
    printf("%s: NCOM: SEND\n", n_int->node_name);
      while (!t_ONTPQ->empty()){
        // Send msg to node
        newTP = t_ONTPQ->popFront();
        // ACK before sending to the same node twice?
        ErrCode = MPI_Send(newTP->get_opr(), 1, MPI_INT, newTP->get_dest_id(), 1, MPI_COMM_WORLD);
        if(!ErrCode){
          printf("%s: NCOM: SEND RD_%03d M_%04d\n", 
          n_int->node_name, newTP->get_dest_id(), *(newTP->get_opr()));
        }else{
          printf("%s: NCOM: SEND RD_%03d M_%04d E_%d\n", 
          n_int->node_name, newTP->get_dest_id(), *(newTP->get_opr()), ErrCode);
        }
        // printf("%s: NCOM: SEND OR(%3d/%3d) DR(%d) EC(%d) m(%3d) \n",
        // n_int->node_name, n_int->node_id, n_int->world_size, newTP->get_dest_id(), ErrCode, *(newTP->get_opr()));
      }   
        
        // Change to IDLE
        n_int->mode_idl();

      break;
    default:
      printf("%s: NCOM: Invalid State\n", n_int->node_name);
    }
  }while (n_int->get_exec());

  // (*nodes_it)->start_NODE();
  // printf("HERE!!!\n");
  // printf("node_id= %d this_node_id= %d\n", (*n_it)->get_id(), n_int->node_id);
  // if ((*n_it)->get_id() != n_int->node_id){}
    
    // n_ext->set_msgbox();

  // for (n_it = nodes.begin(); n_it != nodes.end(); ++n_it){
  //   (*n_it)->setMode();
  // }
  return 0;
}

int NCOM::tst_gen_0()
// TOBEDELETED
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  ThreadedProcedure * newTP;
  // Consuming
  printf("%s: NCOM: Consuming TPs\n", n_int->node_name);
  int x, y;
  x = 0;
  while (x < 3) {
    if (!t_ONTPQ->empty()){
      newTP = t_ONTPQ->popFront();
      y = *(newTP->get_opr());
      printf("%s: NCOM: Popping [%d]\n", n_int->node_name, y);
      ++x;
    }
  }
  usleep(1000000);
  // Producing
  printf("%s: NCOM: Generating TPs\n", n_int->node_name);
  for (x = 0; x < 3; ++x){
    newTP = new ThreadedProcedure();
    newTP->set_orig(n_int->node_id);
    // newTP->set_dest((*n_it)->get_id());
    newTP->set_opr(x);
    printf("%s: NCOM: Pushing [%d]\n", n_int->node_name, x);
    t_INTPQ->push_back(newTP);
    // this_OCTRQ->push_back(x);
  }
  return 0;
}


// TOBEDELETED
  // MPI_Request comm_reqS;
  // MPI_Request comm_reqR;
  // MPI_Status comm_sts;
  // MPI_Status comm_stsR;

  // // Recieve
  // if ((this_node)->node_id == 0){ // NodeA
  //   node_dest = 1;
  //   // ErrCode = MPI_Recv(&Msg_Rec, 1, MPI_INT, node_dest, 5, MPI_COMM_WORLD, &comm_stsR);
  //   MsgDest = &MsgA;
  //   ErrCode = MPI_Irecv(MsgDest, 1, MPI_INT, node_dest, 5, MPI_COMM_WORLD, &comm_reqR);
  //   // printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, node_id, world_size, core_id, core_tot, node_dest, ErrCode);
  //   printf("OPENNING\n");
  // }else if ((this_node)->node_id == 1){ // NodeB
  //   node_dest = 0;
  //   // ErrCode = MPI_Recv(&Msg_Rec, 1, MPI_INT, node_dest, 3, MPI_COMM_WORLD, &comm_stsR);
  //   MsgDest = &MsgB;
  //   ErrCode = MPI_Irecv(MsgDest, 1, MPI_INT, node_dest, 3, MPI_COMM_WORLD, &comm_reqR);
  //   // printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, node_id, world_size, core_id, core_tot, node_dest, ErrCode);
  //   printf("OPENNING\n");
  // }

  // // Send
  // if ((this_node)->node_id == 0){ // NodeA
  //   codeA = 10;
  //   node_dest = 1; 
  //   // ErrCode = MPI_Isend(&codeA, 1, MPI_INT, node_dest, 3, MPI_COMM_WORLD, &comm_reqS);
  //   ErrCode = MPI_Send(&codeA, 1, MPI_INT, node_dest, 3, MPI_COMM_WORLD);
  //   // printf("%s: SENDING msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, codeA, node_id, world_size, core_id, core_tot, node_dest, ErrCode);
  //   printf("SENDING\n");
  // }else if ((this_node)->node_id == 1){ // NodeB
  //   codeB = 11;
  //   node_dest = 0;
  //   // ErrCode = MPI_Isend(&codeB, 1, MPI_INT, node_dest, 5, MPI_COMM_WORLD, &comm_reqS);
  //   ErrCode = MPI_Send(&codeB, 1, MPI_INT, node_dest, 5, MPI_COMM_WORLD);
  //   // printf("%s: SENDING msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, codeB, node_id, world_size, core_id, core_tot, node_dest, ErrCode);
  //   printf("SENDING\n");
  // }

  // // Test
  // if ((this_node)->node_id == 0 || (this_node)->node_id == 1){ // NodeA OR NodeB
  //   // do {
  //     // ErrCode = MPI_Wait(&comm_reqS, &comm_sts);
  //     // ErrCode = MPI_Test(&comm_reqS, &test_flagS, &comm_sts);
  //     // printf("%s: TESTING S[%d] msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, test_flagS, Msg_Rec, node_id, world_size, core_id, core_tot, node_dest, ErrCode);
  //     ErrCode = MPI_Wait(&comm_reqR, &comm_sts);
  //     // ErrCode = MPI_Test(&comm_reqR, &test_flagR, &comm_sts);
  //     // printf("%s: TESTING R[%d] msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, test_flagR, *MsgDest, node_id, world_size, core_id, core_tot, node_dest, ErrCode);
  //     printf("TESTING msg(%d)\n", *MsgDest);
  //     // usleep(1000000);
  //   // } while (!test_flagS && !test_flagR);
  // }




// int NCOM::start()
// {
//   // NCOM initialize channels
//   printf("STARTING NCOM\n");
//   return 0;
// }
