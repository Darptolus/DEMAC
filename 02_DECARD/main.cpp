#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include "node.h"

#include "opr_code.h"
#include "thread_safe_deque.h"

#include <unistd.h>

#include <e-loader.h>
#include <e-hal.h>  // Epiphany Hardware Abstraction Layer
                    // functionality for communicating with epiphany chip when
                    // the application runs on a host, typically the ARM µp

#define BUFOFFSET (0x01000000)  // SDRAM is at 0x8f00'0000,
                                // offset in e_read starts at 0x8e00'0000

unsigned rows, cols, i, j, ncores, row, col;

/*
 * Init the epiphany platform
 */
 
void init_epiphany(e_platform_t * platform) {
  e_init(NULL);
  e_reset_system();
  e_get_platform_info(platform);
}

/*
 * Create the workgroup and load programs into it
 */
int init_workgroup(e_epiphany_t * dev) {
  e_return_stat_t result;
  e_open(dev, 0, 0, rows, cols); // Create an epiphany cores workgroup
  e_reset_group(dev);
  // load programs into cores workgroup, do not execute it immediately
  result = static_cast<e_return_stat_t>(e_load_group("emain.elf", dev, 0, 0, rows, cols, E_FALSE));
  if(result != E_OK) {
    printf("Error Loading the Epiphany Application %i\n", result);
  }
  e_start_group(dev);
}

//void com();

int main(int argc, char* argv[]) {

  //printf(">>>>> New <<<<<\n");
  // Initialize MPI
  MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char node_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(node_name, &name_len);

    MPI_Request comm_req[world_size];
    MPI_Status comm_sts[world_size];

//printf("%s: rank %d\t from %d\n",node_name, world_rank, world_size);

  bool exec_prog = 1;
  bool end_all = 0;               // Signal finish execution
  bool exe_end = 1;               // Indicates when all codelets have been executed

  
  // Define thread safe queues
  thread_safe::deque<ThreadedProcedure*> INTPQ;
  thread_safe::deque<ThreadedProcedure*> ONTPQ;
  thread_safe::deque<ThreadedProcedure*> ISTPQ;
  thread_safe::deque<ThreadedProcedure*> OSTPQ;

  //* int TP_count = 0; // Total number of TPs
  //* int TP_c_inS = 0; // Number of TPs for the current node
  int TP_c_out = 0; // Number of TPs to be sent to other nodes
  //* int TP_com_h = 0; // Header pointer of the TP comm

  int r_flag = 0;
  int t_flag = 0;
  int c_flag = 0;
  int s_flag = 0;
  int i_flag = 0;
  int end_msg_id;
  int init_code = 1; // Can potentially include security features 

  struct timeval start, end;

  int e_flag_n[2];
  int e_flag_t[2][world_size];

  nodeComm ComMod[world_size]; 
  comm_mode c_mode;
  
  e_platform_t platform;  // platform infos
  e_epiphany_t dev;       // provides access to cores workgroup
  e_mem_t emem;           // shared memory buffer

gettimeofday(&start, NULL);

    init_epiphany(&platform);

    rows = platform.rows;
    cols = platform.cols;
    ncores = rows * cols;
    uint32_t result[ncores];     // to store the results, size of cores
    // allocate a space to share data between e_cores and here
    // offset starts from 0x8e00'0000
    // sdram (shared space) is at 0x8f00'0000
    // so 0x8e00'0000 + 0x0100'0000 = 0x8f00'0000
    e_alloc(&emem, BUFOFFSET, ncores*sizeof(uint32_t));
    init_workgroup(&dev);

MPI_Barrier(MPI_COMM_WORLD);

#pragma omp parallel num_threads(2)
{
  int core_id = omp_get_thread_num();
  int core_tot = omp_get_num_threads();
  
// Node Manager (NMGR)
  if (core_id == 1){ // Scheduler

//****************************************************************//
    printf("The Scheduler in %s, in core %d from %d\n",node_name, core_id, core_tot);
//****************************************************************//
    ThreadedProcedure* newTP;
    if (world_rank == 0){ // Node01
//Generate TPs
      for (int i = 0; i< world_size; i++){
          newTP = new ThreadedProcedure();
          newTP->parent_n = world_rank;
          newTP->child_n = i;
          newTP->opr_id = GMM_ID;
        if (i == world_rank){
          INTPQ.push_back(newTP);
        }else{
          ONTPQ.push_back(newTP);
        }

//****************************************************************//
//         // printf("\033[22;34m%s S(%d):\033[0m GEN mode [TP(%d).p(%d) msg(%d) for Rank: %d \tout(%d) in(%d)]\n",
//         //node_name, core_id, TP_count, i, newTP->opr_id, newTP->child_n, TP_c_out, TP_c_inS);
        printf("%s S(%d): GEN mode [TPQ_out(%d)/TPQ_in(%d) msg(%d) for Rank: %d]\n",
        node_name, core_id, ONTPQ.size(), INTPQ.size(), newTP->opr_id, newTP->child_n);
// //****************************************************************//
      
      }
        newTP = new ThreadedProcedure();
        newTP->parent_n = world_rank;
        newTP->child_n = -1;
        newTP->opr_id = END_ID;
        INTPQ.push_back(newTP);
        ONTPQ.push_back(newTP);

//****************************************************************//
        printf("%s S(%d): GEN mode [TPQ_out(%d)/TPQ_in(%d) msg(%d) Broadcast END PROG]\n",
        node_name, core_id, ONTPQ.size(), INTPQ.size(), newTP->opr_id);
//****************************************************************//

    }else{
      // Local END TP generation 
      newTP = new ThreadedProcedure();
      newTP->parent_n = world_rank;
      newTP->child_n = world_rank;
      newTP->opr_id = END_ID;
      ONTPQ.push_back(newTP);
//********************************************************************************************************************************//
      printf("%s S(%d): OWN mode [TPQ_out(%d)/TPQ_in(%d) msg(%d) for Rank: %d]\n",
      node_name, core_id, ONTPQ.size(), INTPQ.size(), newTP->opr_id, newTP->child_n);
//********************************************************************************************************************************//
    }

    usleep(100);
    // we read from the allocated space and store it to the result array
    e_read(&emem, 0, 0, 0x0, &result, ncores * sizeof(uint32_t)); // reads what's ben put in buffer
    for(row = 0; row < rows; row++) {
      for(col = 0; col < cols; col++) {
        fprintf(stdout, "[%i]", result[row*cols+col]);
      }
    }
    fprintf(stdout, "\n");
    fflush(stdout);
    
    e_close(&dev);


// // Node Communicator
  }else if (core_id == 0){ // Comms
    
// // Define Rol // Initiallize comms  
    for(int i=0; i<world_size; i++){
      if (i == 0){
        ComMod[i].n_mode = SEND;
      }else{
        ComMod[i].n_mode = RECEIVE;
      }
      ComMod[i].R_set = 0;
      ComMod[i].R_rec = 0;
      comm_req[i] = 0;
    }
    c_mode = RECEIVE;

//********************************************************************************************************************************//
    //usleep(1000000);
    printf("The Communicator in %s, in core %d from %d is in %d mode\n",node_name, core_id, core_tot, c_mode);
//********************************************************************************************************************************//

    while(exec_prog){
      switch(c_mode){
        case INIT:
//********************************************************************************************************************************//
// Flags the first time that Comms enters Receive
          if (i_flag == 0){
            printf("%s C(%d): INIT mode [INIT COMMS TPQ_out(%d)/TPQ_in(%d)]\n",
            node_name, core_id, ONTPQ.size(), INTPQ.size());
            r_flag = 1;
          }
//********************************************************************************************************************************//
          for (int i = 0; i< world_size; i++){
            //Recieve
//********************************************************************************************************************************//
            printf("%s C(%d): REC mode [Opening comms for Rank: %d]\n", node_name, core_id, i);
//********************************************************************************************************************************//
            MPI_Irecv(&ComMod[i].Msg_Rec, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &comm_req[i]);
          }

          for (int i = 0; i< world_size; i++){
            //Send
            MPI_Send(&init_code, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("%s C(%d): INI mode [INIT COMMS TPQ_out(%d)/TPQ_in(%d)]\n", node_name, core_id, ONTPQ.size(), INTPQ.size());
          }

          for (int i = 0; i< world_size; i++){
            //Test

          }


        case RECEIVE:
//********************************************************************************************************************************//
// Flags the first time that Comms enters Receive
          if (r_flag == 0){
            printf("%s C(%d): REC mode [INIT RECEIVE TPQ_out(%d)/TPQ_in(%d)]\n", node_name, core_id, ONTPQ.size(), INTPQ.size());
            r_flag = 1;
          }
//********************************************************************************************************************************//

// Check NCOM reciever status 
          for (int i = 0; i< world_size; i++){
            if (i!=world_rank && !ComMod[i].R_set){
// Comms not open -> Open comms 
// Non Blocking Receive
              MPI_Irecv(&ComMod[i].Msg_Rec, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &comm_req[i]);
              ComMod[i].R_set = 1;
              // ComMod[i].R_rec = 1;

//********************************************************************************************************************************//
              printf("%s C(%d): REC mode [Opening comms for Rank: %d]\n", node_name, core_id, i);
//********************************************************************************************************************************//

            } else if (i!=world_rank && ComMod[i].R_set && !ComMod[i].R_rec){
// Comms open -> Check Recieve status
// Flags the first time that Comms check Receive status
              if (ComMod[i].T_flag == 0){
                ComMod[i].T_flag = 1;
//********************************************************************************************************************************//
                printf("%s C(%d): REC mode [Test comms for Rank: %d]\n", node_name, core_id, i);
//********************************************************************************************************************************//
              }
              MPI_Test(&comm_req[i], &ComMod[i].R_rec, &comm_sts[i]);
              //ComMod[i].R_rec = 1;
            }

// Check for message received
            if (i!=world_rank && ComMod[i].R_rec){
//********************************************************************************************************************************//
              printf("%s C(%d): REC mode [Message: (%d) recieved from Rank: %d]\n",  node_name, core_id, ComMod[i].Msg_Rec, i);
//********************************************************************************************************************************//
// Message Recieved -> Push to INTPQ
//################################################################//
//################################################################//
              ComMod[i].R_set = 0;
              ComMod[i].R_rec = 0;
              ComMod[i].T_flag = 0;
              if(ComMod[i].Msg_Rec == END_ID){
                end_all = 1;
              }

            }
          }
          
          if(INTPQ.size() == 0 && ONTPQ.size() == 0){
//****************************************************************//
            printf("%s C(%d): Queues Empty\n", node_name, core_id);
//****************************************************************//
            if(end_all && exe_end){
//****************************************************************//
              printf("%s C(%d): END of the program\n", node_name, core_id);
//****************************************************************//
              exec_prog = 0;
            }
          }

// Check if comms have been initialized
          if(!i_flag){
            c_mode = INIT;
//****************************************************************//
            printf("%s C(%d): REC mode [Changing to INIT mode TPQ_out(%d)/TPQ_in(%d)]\n", node_name, core_id, ONTPQ.size(), INTPQ.size());
//****************************************************************// 
// Check if there are outgoing TPs, mode needs to be changed to SEND 
          }else if (ONTPQ.size() >= MIN_TP_NUM){ // MIN_TP_NUM = 1
// Broadcast change of mode
            c_mode = SEND;
//****************************************************************//
            printf("%s C(%d): REC mode [Changing to SND mode TPQ_out(%d)/TPQ_in(%d)]\n", node_name, core_id, ONTPQ.size(), INTPQ.size());
//****************************************************************//    
            //MPI_Bcast(&SchTPQ.tp[TP_c_out].opr_code, 1, MPI_INT, SchTPQ.tp[TP_c_out].parent, MPI_COMM_WORLD);
            s_flag = 0; // Flags the first time NCOM enters Send

            // c_flag = 0;
        break; // *End RECEIVE*
          }
          
        case SEND:
// Flag to message the first time that NCOM enters Send
          if (s_flag == 0){
//****************************************************************//
            printf("%s C(%d): SND mode [INIT SEND TPQ_out(%d)/TPQ_in(%d)]\n", node_name, core_id, ONTPQ.size(), INTPQ.size());
//****************************************************************//
            s_flag = 1;
          }
          while(!c_flag){
            ThreadedProcedure* CommTP = ONTPQ.popFront();
//****************************************************************//
            printf("%s C(%d): SND mode [Msg(%d) TPQ_out(%d)/TPQ_in(%d)]\n",node_name, core_id, CommTP->opr_id, ONTPQ.size(), INTPQ.size());
//****************************************************************//
      
            if (CommTP != NULL){
// 
              if (CommTP->child_n == -1){
// Broadcast
//********************************************************************************************************************************//
                  printf("%s C(%d): SND mode [Broadcast(%d) TPQ_out(%d)/TPQ_in(%d)]\n",
                  node_name, core_id, CommTP->opr_id, ONTPQ.size(), INTPQ.size());
//********************************************************************************************************************************//
//                   //MPI_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)
//                   //MPI_Bcast(&SchTPQ.tp[TP_c_out].opr_code, 1, MPI_INT, SchTPQ.tp[TP_c_out].parent, MPI_COMM_WORLD);
                  for (int j = 0; j< world_size; j++){
                    if (j!=world_rank){
//                 //       printf("%s C(%d): BCT mode [Sending %d to Rank: %d TPQ_out(%d)/TPQ_in(%d)]\n",
//                 //       node_name, core_id, CommTP->opr_id, j, ONTPQ.size(), INTPQ.size());
                      
                      //MPI_Send(&CommTP->opr_id, 1, MPI_INT, j, 0, MPI_COMM_WORLD);
                    }
                  }
//                 //   if (CommTP->opr_id == END_ID){
//                 //     ComMod[world_rank].Msg_Rec = END_ID;
//                 //   }
                  
              }else if (CommTP->child_n != world_rank){
// Send to NODE##
//********************************************************************************************************************************//                
                printf("%s C(%d): SND mode [Sending %d to Rank: %d TPQ_out(%d)/TPQ_in(%d)]\n",
                node_name, core_id, CommTP->opr_id, CommTP->child_n, ONTPQ.size(), INTPQ.size());
//********************************************************************************************************************************//
                //MPI_Send(&CommTP->opr_id, 1, MPI_INT, CommTP->child_n, 0, MPI_COMM_WORLD);
              }else if(CommTP->child_n == world_rank){
// Own NODE
//********************************************************************************************************************************//
                  printf("%s C(%d): SND mode [Detected %d for OWN node Rank: %d TPQ_out(%d)/TPQ_in(%d)]\n",
                  node_name, core_id, CommTP->opr_id, CommTP->child_n, ONTPQ.size(), INTPQ.size());
//********************************************************************************************************************************//
              }
            }else{
              c_flag = 1; // Flag completed send process
            }
// Check if succesful receive or missing receive
            }// *End while*
            c_mode = RECEIVE;
            r_flag = 0;
            c_flag = 0;
            //usleep(1000000);
        break; // *End SEND*
        
      } // *End Switch*
    } // *End while*
    printf("%s C(%d): END of the program\n",node_name, core_id);
  } // *End Comms*

  // e_flag_n[core_id] = 1; // 1 = ok
//   //MPI_Gather(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator)
  
//   //if (core_id == 0 && world_rank == 0){
//   //  MPI_Gather(&e_flag_n[core_id], 1, MPI_INT, &e_flag_t[core_id][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
//   //  MPI_Gather(&e_flag[world_rank][1], 1, MPI_INT, &e_flag[world_rank][1], 1, MPI_INT, 0, MPI_COMM_WORLD);
//   //}
//   //MPI_Allgather(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, int recv_count, MPI_Datatype recv_datatype, MPI_Comm communicator)
//   //MPI_Allgather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, MPI_COMM_WORLD);
  
//   //printf("<<<<<%s: End of the program rank %d\t from %d, core: %d >>>>>\n",node_name, world_rank, world_size, core_id);

#pragma omp barrier
}

gettimeofday(&end, NULL);

printf("%s: %lf us\n", node_name, (((end.tv_sec * 1000000 + end.tv_usec)
        - (start.tv_sec * 1000000 + start.tv_usec)))*1.0);

gettimeofday(&start, NULL);
fflush(stdout);

MPI_Barrier(MPI_COMM_WORLD);

MPI_Gather(&e_flag_n[0], 1, MPI_INT, &e_flag_t[0][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Gather(&e_flag_n[1], 1, MPI_INT, &e_flag_t[1][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);

MPI_Barrier(MPI_COMM_WORLD);

gettimeofday(&end, NULL);

if (world_rank == 0){
  for(int i = 0; i< world_size; i++){
    printf("%d:\t",i);
    for(int j = 0; j< 2; j++){
      printf("[%d]", e_flag_t[j][i]);
    }
    printf("\n");
  }
  printf("Time 2: %lf us\n", (((end.tv_sec * 1000000 + end.tv_usec)
        - (start.tv_sec * 1000000 + start.tv_usec)))*1.0);
}

MPI_Finalize();
return 0;
}
