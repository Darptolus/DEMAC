#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <mpi.h>

int main(){

int codeA = 0;
int codeB = 0;
int node_dest = 0;
int Msg_Rec = 0;

MPI_Init(NULL, NULL);
		int world_size;
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		int world_rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

		char node_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Get_processor_name(node_name, &name_len);

    MPI_Request comm_reqS;
    MPI_Request comm_reqR;
    MPI_Status comm_sts;
	
	int e_flag_n[4];
	int e_flag_t[4][world_size];

	struct timeval start_all, end_all;
	struct timeval start, end;
	
gettimeofday(&start_all, NULL);
gettimeofday(&start, NULL);

#pragma omp parallel num_threads(2)
{
	int core_id = omp_get_thread_num();
  int core_tot = omp_get_num_threads();

  //printf("%s: R(%d/%d)C(%d/%d)\n", node_name, world_rank, world_size, core_id, core_tot);
  e_flag_n[core_id] = 1; // 1 = ok
	e_flag_n[core_id+2] = world_rank; // 1 = ok

  if (core_id == 0){ // Comms

// Recieve
    if (world_rank == 0){ // NodeA
      codeA = 10;
      node_dest = 1;
      MPI_Irecv(&Msg_Rec, 1, MPI_INT, node_dest, 0, MPI_COMM_WORLD, &comm_reqR);
      printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \n", node_name, world_rank, world_size, core_id, core_tot, node_dest);
    }else if (world_rank == 1){ // NodeB
      codeB = 11;
      node_dest = 0;
      MPI_Irecv(&Msg_Rec, 1, MPI_INT, node_dest, 0, MPI_COMM_WORLD, &comm_reqR);
      printf("%s: OPENNING \t\t in R(%d/%d)C(%d/%d) \t to R(%d) \n", node_name, world_rank, world_size, core_id, core_tot, node_dest);
    }

// Send
    if (world_rank == 0){ // NodeA
      codeA = 10;
      node_dest = 1;
      MPI_Isend(&codeA, 1, MPI_INT, node_dest, 0, MPI_COMM_WORLD, &comm_reqS);
      printf("%s: SENDING msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \n", node_name, codeA, world_rank, world_size, core_id, core_tot, node_dest);
    }else if (world_rank == 1){ // NodeB
      codeB = 11;
      node_dest = 0;
      MPI_Isend(&codeB, 1, MPI_INT, node_dest, 0, MPI_COMM_WORLD, &comm_reqS);
      printf("%s: SENDING msg(%d) \t from R(%d/%d)C(%d/%d) \t to R(%d) \n", node_name, codeB, world_rank, world_size, core_id, core_tot, node_dest);
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