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
  int MsgA = 0;
  int MsgB = 0;
  int* MsgDest;
  int ErrCode = 0;

  int test_flagS = 0;
  int test_flagR = 0;

  MPI_Request comm_reqS;
  MPI_Request comm_reqR;
  MPI_Status comm_sts;
  MPI_Status comm_stsR;

  // Recieve
  if ((this_node)->world_rank == 0){ // NodeA
    node_dest = 1;
    // ErrCode = MPI_Recv(&Msg_Rec, 1, MPI_INT, node_dest, 5, MPI_COMM_WORLD, &comm_stsR);
    MsgDest = &MsgA;
    ErrCode = MPI_Irecv(MsgDest, 1, MPI_INT, node_dest, 5, MPI_COMM_WORLD, &comm_reqR);
    // printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, world_rank, world_size, core_id, core_tot, node_dest, ErrCode);
    printf("OPENNING\n");
  }else if ((this_node)->world_rank == 1){ // NodeB
    node_dest = 0;
    // ErrCode = MPI_Recv(&Msg_Rec, 1, MPI_INT, node_dest, 3, MPI_COMM_WORLD, &comm_stsR);
    MsgDest = &MsgB;
    ErrCode = MPI_Irecv(MsgDest, 1, MPI_INT, node_dest, 3, MPI_COMM_WORLD, &comm_reqR);
    // printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, world_rank, world_size, core_id, core_tot, node_dest, ErrCode);
    printf("OPENNING\n");
  }

  // Send
  if ((this_node)->world_rank == 0){ // NodeA
    codeA = 10;
    node_dest = 1; 
    // ErrCode = MPI_Isend(&codeA, 1, MPI_INT, node_dest, 3, MPI_COMM_WORLD, &comm_reqS);
    ErrCode = MPI_Send(&codeA, 1, MPI_INT, node_dest, 3, MPI_COMM_WORLD);
    // printf("%s: SENDING msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, codeA, world_rank, world_size, core_id, core_tot, node_dest, ErrCode);
    printf("SENDING\n");
  }else if ((this_node)->world_rank == 1){ // NodeB
    codeB = 11;
    node_dest = 0;
    // ErrCode = MPI_Isend(&codeB, 1, MPI_INT, node_dest, 5, MPI_COMM_WORLD, &comm_reqS);
    ErrCode = MPI_Send(&codeB, 1, MPI_INT, node_dest, 5, MPI_COMM_WORLD);
    // printf("%s: SENDING msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, codeB, world_rank, world_size, core_id, core_tot, node_dest, ErrCode);
    printf("SENDING\n");
  }

  // Test
  if ((this_node)->world_rank == 0 || (this_node)->world_rank == 1){ // NodeA OR NodeB
    // do {
      // ErrCode = MPI_Wait(&comm_reqS, &comm_sts);
      // ErrCode = MPI_Test(&comm_reqS, &test_flagS, &comm_sts);
      // printf("%s: TESTING S[%d] msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, test_flagS, Msg_Rec, world_rank, world_size, core_id, core_tot, node_dest, ErrCode);
      ErrCode = MPI_Wait(&comm_reqR, &comm_sts);
      // ErrCode = MPI_Test(&comm_reqR, &test_flagR, &comm_sts);
      // printf("%s: TESTING R[%d] msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, test_flagR, *MsgDest, world_rank, world_size, core_id, core_tot, node_dest, ErrCode);
      printf("TESTING\n");
      usleep(1000000);
    // } while (!test_flagS && !test_flagR);
  }

  return 0;
}

// int NCOM::start()
// {
//   // NCOM initialize channels
//   printf("STARTING NCOM\n");
//   return 0;
// }
