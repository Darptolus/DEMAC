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
  // NCOM initialize channels
  printf("INITIALIZING NCOM\n");
  // for (n_it = nodes.begin(); n_it != nodes.end(); ++n_it){
  //   (*n_it)->setMode();
  // }
  int codeA = 0;
  int codeB = 0;
  int node_dest = 0;
  int MsgA = 10;
  int MsgB = 0;
  int* MsgDest;
  int ErrCode = 0;

  int test_flagS = 0;
  int test_flagR = 0;
  Node_Intern * t_node = (Node_Intern *) this_node;
  Node_Extern * n_ext;


  // Recieve
  for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
    // (*nodes_it)->start_NODE();
    // printf("HERE!!!\n");
    // printf("node_id= %d this_node_id= %d\n", (*n_it)->get_id(), (t_node)->node_id);
    if ((*n_it)->get_id() != (t_node)->node_id){
      n_ext = dynamic_cast <Node_Extern *> (*n_it);
      // n_ext->set_msgbox();
      printf("%s: OPENNING \t\t in R(%d/%d) \t to R(%d) \t ErrCode(%d)\n",
      (t_node)->node_name, (t_node)->node_id, (t_node)->world_size, n_ext->get_id(), ErrCode);
      ErrCode = MPI_Irecv(n_ext->get_msgbox(), 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD, n_ext->get_rreq()); 
    }
  }

  // Send
  for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
    if ((*n_it)->get_id() != (t_node)->node_id){
      n_ext = dynamic_cast <Node_Extern *> (*n_it);
      codeA = 10*((*n_it)->get_id() +1);
      ErrCode = MPI_Send(&codeA, 1, MPI_INT, n_ext->get_id(), 1, MPI_COMM_WORLD);
      printf("%s: SENDING msg(%d) \t from R(%d/%d) \t to R(%d) \t ErrCode(%d)\n", 
      (t_node)->node_name, codeA, (t_node)->node_id, (t_node)->world_size, n_ext->get_id(), ErrCode);
    }
  }

  // Test
  for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
    if ((*n_it)->get_id() != (t_node)->node_id){
      n_ext = dynamic_cast <Node_Extern *> (*n_it);
      ErrCode = MPI_Wait(n_ext->get_rreq(), n_ext->get_rsts());
      // ErrCode = MPI_Test(&((t_node)->rcv_req), &test_flagR, &((t_node)->rcv_sts));
      printf("%s: TESTING R[%d] msg(%d) \t from R(%d/%d) \t to R(%d) \t ErrCode(%d)\n", 
      (t_node)->node_name, test_flagR, *(n_ext->get_msgbox()), (t_node)->node_id, (t_node)->world_size, n_ext->get_id(), ErrCode);
      usleep(1000000);
    }
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


  return 0;
};

// int NCOM::start()
// {
//   // NCOM initialize channels
//   printf("STARTING NCOM\n");
//   return 0;
// }
