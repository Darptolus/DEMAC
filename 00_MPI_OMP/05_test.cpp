// Test 05
// IRecv / Send / Test
// 3 nodes
// 2 messages (array) per node P2P 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <mpi.h>
#include <unistd.h>

int main(){

int Msg[10];
int MsgA[10];
int MsgB[10];
int MsgC[10];

int* MsgDest;
int* MsgOutb;
int Msg_All[3][10] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
int ErrCode = 0;
int i = 0, j = 0;
int checkPass = 1;

MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char node_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(node_name, &name_len);

    MPI_Request comm_reqS[world_size];
    MPI_Request comm_reqR[world_size];
    MPI_Status comm_sts[world_size];
    MPI_Status comm_stsR[world_size];

    int test_flagS[world_size] = {0, 0, 0};
    int test_flagR[world_size] = {0, 0, 0};

	
	int e_flag_n[4];
	int e_flag_t[4][world_size];

	struct timeval start_all, end_all;
	struct timeval start, end;
	
gettimeofday(&start_all, NULL);
gettimeofday(&start, NULL);

#pragma omp parallel num_threads(2) default(shared)
{
	int core_id = omp_get_thread_num();
  int core_tot = omp_get_num_threads();

  //printf("%s: R(%d/%d)C(%d/%d)\n", node_name, world_rank, world_size, core_id, core_tot);
  e_flag_n[core_id] = 1; // 1 = ok
  e_flag_n[core_id+2] = world_rank; // 1 = ok

  if (core_id == 0){ // Comms
    if (world_rank == 0 || world_rank == 1 || world_rank == 2){ // NodeA OR NodeBOR NodeC

// Init messages
      for (i = 0; i < 10; i++){
        Msg[i] = (i + 1) * world_rank;
        Msg_All[world_rank][i] = (i + 1) * world_rank;
      }
      printf("%s: GEN_MSG \t\t R(%d/%d)C(%d/%d) \t to R(ALL) \t ErrCode(%d)\n", node_name, world_rank, world_size, core_id, core_tot, ErrCode); 

// Recieve
      for(i = 0; i < world_size ; i++ ){
        if (i != world_rank){
          if (i == 0){
            MsgDest = &MsgA[0];
            // ErrCode = MPI_Irecv(&MsgA[0], 10, MPI_INT, i, 1, MPI_COMM_WORLD, &comm_reqR[i]);
            // printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, world_rank, world_size, core_id, core_tot, i, ErrCode);
          }else if (i == 1){
            MsgDest = &MsgB[0];
            // ErrCode = MPI_Irecv(&MsgB[0], 10, MPI_INT, i, 1, MPI_COMM_WORLD, &comm_reqR[i]);
            // printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, world_rank, world_size, core_id, core_tot, i, ErrCode);
          }else if (i == 2){
            MsgDest = &MsgC[0];
            // ErrCode = MPI_Irecv(&MsgC[0], 10, MPI_INT, i, 1, MPI_COMM_WORLD, &comm_reqR[i]);
            // printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, world_rank, world_size, core_id, core_tot, i, ErrCode);
          }
          ErrCode = MPI_Irecv(MsgDest, 10, MPI_INT, i, 1, MPI_COMM_WORLD, &comm_reqR[i]);
          printf("%s: OPENNING \t\t R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, world_rank, world_size, core_id, core_tot, i, ErrCode);
        }
      }
      // usleep(1000000);
      // MPI_Barrier(MPI_COMM_WORLD);

// Send
      for(i = 0; i < world_size ; i++ ){
        if (i != world_rank){
          ErrCode = MPI_Send(Msg, 10, MPI_INT, i, 1, MPI_COMM_WORLD);
          printf("%s: SENDING msg(%d) \t\t R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, *MsgOutb, world_rank, world_size, core_id, core_tot, i, ErrCode);
        }
      }

// Test
      do {
        // ErrCode = MPI_Wait(&comm_reqR, &comm_sts);
        for(i = 0; i < world_size ; i++ ){
          if (i == world_rank && !test_flagR[i]){
            test_flagR[i]=1;
          }else if (i != world_rank && !test_flagR[i]){
            ErrCode = MPI_Test(&comm_reqR[i], &test_flagR[i], &comm_sts[i]);
            if (i == 0){
              MsgDest = &MsgA[0];
            }else if (i == 1){
              MsgDest = &MsgB[0];
            }else if (i == 2){
              MsgDest = &MsgC[0];
            }
            printf("%s: TEST[%d] msg(%d) \t R(%d/%d)C(%d/%d) \t to R(%d) \t ErrCode(%d)\n", node_name, test_flagR[i], *MsgDest, world_rank, world_size, core_id, core_tot, i, ErrCode);
            // usleep(1000000);
          }
        }
      } while (!(test_flagR[0] && test_flagR[1] && test_flagR[2]));
    
// Assemble messages
      for (i = 0; i < 10; i++){
        if (world_rank != 0){
          Msg_All[0][i] = MsgA[i];
        }else if (world_rank != 1){
          Msg_All[1][i] = MsgB[i];
        }else if (world_rank != 2){
          Msg_All[2][i] = MsgC[i];
        }
      }

// Check Message Integrity
      for (i = 0; i < 10; i++){
        for (i = 0; i < 10; i++){
          if (Msg_All[j][i] != (i + 1) * j){
            checkPass = 0;
          }
        }
      }
      
      if (checkPass == 1){
        printf("%s: PASS \t\t in R(%d/%d)C(%d/%d)\n", node_name, world_rank, world_size, core_id, core_tot);
      }else if (checkPass == 0){
        printf("%s: FAIL \t\t in R(%d/%d)C(%d/%d)\n", node_name, world_rank, world_size, core_id, core_tot); 
      }
    }
  }
	#pragma omp barrier
}

gettimeofday(&end, NULL);

printf("%s: %lf us\n", node_name, (((end.tv_sec * 1000000 + end.tv_usec)
				- (start.tv_sec * 1000000 + start.tv_usec)))*1.0);

//gettimeofday(&start, NULL);
fflush(stdout);

MPI_Barrier(MPI_COMM_WORLD);

MPI_Gather(&e_flag_n[0], 1, MPI_INT, &e_flag_t[0][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Gather(&e_flag_n[1], 1, MPI_INT, &e_flag_t[1][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Gather(&e_flag_n[2], 1, MPI_INT, &e_flag_t[2][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Gather(&e_flag_n[3], 1, MPI_INT, &e_flag_t[3][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);

MPI_Barrier(MPI_COMM_WORLD);

gettimeofday(&end_all, NULL);

if (world_rank == 0){
	for(int j = 0; j< sizeof(e_flag_n)/sizeof(int); j++){
		for(int i = 0; i< world_size; i++){
			//printf("%d:\n",i);		
			printf("[%02d]", e_flag_t[j][i]);
		}
		printf("\n");
	}
	printf("Total Time: %lf us\n", (((end_all.tv_sec * 1000000 + end_all.tv_usec)
				- (start_all.tv_sec * 1000000 + start_all.tv_usec)))*1.0);
}

MPI_Finalize();
return 0;
}