#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include "decard.hpp"

int main(int argc, char *argv[]){
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char node_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(node_name, &name_len);

  #pragma omp parallel num_threads(2)
  {
    int core_id = omp_get_thread_num();
    int core_tot = omp_get_num_threads();

    //printf("%s: R(%d/%d) C(%d/%d)\n", node_name, world_rank+1, world_size, core_id+1, core_tot);
    // printf("C(%d/%d)\n", core_id, core_tot);

    if (core_id == 0){
      printf("%s: This is NCOM R(%d/%d) C(%d/%d)\n", node_name, world_rank+1, world_size, core_id+1, core_tot);
    }else if (core_id == 1){
      printf("%s: This is NMGR R(%d/%d) C(%d/%d)\n", node_name, world_rank+1, world_size, core_id+1, core_tot);
    }

    #pragma omp barrier
  }

  start_DECARD();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();

}