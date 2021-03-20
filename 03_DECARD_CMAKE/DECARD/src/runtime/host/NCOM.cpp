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
  // todo: t_node -> n_int
  Node_Extern * n_ext;
  int codeA = 0;
  int codeB = 0;
  int node_dest = 0;
  int MsgA = 10;
  int MsgB = 0;
  int* MsgDest;
  int ErrCode = 0;

  int test_flagS = 0;
  int test_flagR = 0;

  // NCOM initialize channels
  printf("%s: INITIALIZING NCOM\n", n_int->node_name);

/***************************************************
  
  for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
    // Clear channel availability
    (*n_it)->clr_renb()
  }

  do{
    switch(n_int->get_mode()) {
    case N_DONE:
      printf("%s: NCOM DONE\n", n_int->node_name);
      // Send DONE msg

      // set exec = 0
      break;
    case N_IDLE:
      printf("%s: NCOM IDLE\n", n_int->node_name);
      // Test
      for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
        if ((*n_it)->get_id() != n_int->node_id){
          n_ext = dynamic_cast <Node_Extern *> (*n_it);
          // Check if msg has been recieved
          // ErrCode = MPI_Wait(n_ext->get_rreq(), n_ext->get_rsts());
          ErrCode = MPI_Test(n_ext->get_rreq(), n_ext->get_rflg(), n_ext->get_rsts());
          printf("%s: TESTING R[%d] msg(%d) \t from R(%d/%d) \t to R(%d) \t ErrCode(%d)\n", 
          n_int->node_name, *(n_ext->get_rflg()), *(n_ext->get_msgbox()), n_int->node_id, n_int->world_size, n_ext->get_id(), ErrCode);
        }
        if(*(n_ext->get_rflg())){
          // Mssage recieved -> Set origin node -> Change to RECEIVE
          n_int->set_nrcv(n_ext);
          n_ext->clr_renb();
          n_int->mode_rcv();
          break;
        }else if(n_ext->get_msgout()){ // ********** Msg in q? **********
          // Outgoing message(s) -> Change to SEND
          n_int->mode_snd();
          break;
        }
      }
      if(n_int->get_mode() == N_IDLE){
        // Stay in idle
        usleep(1000000);
      }
      break;

    case N_RECEIVE:
      // Recieve
      printf("%s: NCOM RECEIVE\n", n_int->node_name);
      // Check for incoming message
      if(n_int->get_nrcv() != NULL){
        // Message Recieved
        printf("%s: NCOM: Pushing [%d]\n", n_int->node_name, *(n_ext->get_msgbox())); // *((n_int->get_nrcv())->get_msgbox())
        // Push to queue
        this_ICTRQ->push_back(*(n_ext->get_msgbox()));
        // Clear Recieved
        n_int->set_nrcv(NULL);
        // Reenable Channel
        printf("%s: OPENNING \t\t in R(%d/%d) \t to R(%d) \t ErrCode(%d)\n",
        n_int->node_name, n_int->node_id, n_int->world_size, n_ext->get_id(), ErrCode);
        ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
        n_ext->set_renb();
      }else{
      // Check all channels
        for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
          // Check channel availability
          if (!((*n_it)->get_renb) && ((*n_it)->get_id() != n_int->node_id)){
            // Open channel
            n_ext = dynamic_cast <Node_Extern *> (*n_it);
            printf("%s: OPENNING \t\t in R(%d/%d) \t to R(%d) \t ErrCode(%d)\n",
            n_int->node_name, n_int->node_id, n_int->world_size, n_ext->get_id(), ErrCode);
            ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
            n_ext->set_renb();
          }
        }
      }
      if(n_ext->get_msgout()){
          // Outgoing message(s) -> Change to SEND
          n_int->mode_snd();
        }else{
          // Change to SEND
          n_int->mode_idl();
        }

      // set idle mode
      break;
    case N_SEND:
      printf("%s: NCOM SEND\n", n_int->node_name);
      // Recieve message & push to queue

      // Send msg to node
      for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
        if ((*n_it)->get_id() != n_int->node_id){
          n_ext = dynamic_cast <Node_Extern *> (*n_it);
          codeA = 10*((*n_it)->get_id() +1);
          ErrCode = MPI_Send(&codeA, 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD);
          printf("%s: SENDING msg(%d) \t from R(%d/%d) \t to R(%d) \t ErrCode(%d)\n", 
          n_int->node_name, codeA, n_int->node_id, n_int->world_size, n_ext->get_id(), ErrCode);
        }
      } 

      // set idle mode
      break;
    default:
      printf("%s: NCOM Invalid State\n", n_int->node_name);
    }
  }while(n_int->get_exec())


**************************************************/

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
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  ThreadedProcedure * newTP;
  // Consuming
  printf("%s: NCOM: Consuming TPs\n", n_int->node_name);
  int x, y;
  x = 0;
  while (x < 3) {
    if(!t_ONTPQ->empty()){
      newTP = t_ONTPQ->popFront();
      y = newTP->get_opr();
      printf("%s: NCOM: Popping [%d]\n", n_int->node_name, y);
      ++x;
    }
  }
  usleep(1000000);
  // Producing
  printf("%s: NCOM: Generating TPs\n", n_int->node_name);
  for(x = 0; x < 3; ++x){
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
