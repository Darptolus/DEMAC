#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

#include "AbstractMachine.hpp"

int world_size;
int world_rank;
char node_name[16];
int name_len;

int start_AM()
{
  // Initialize AbstractMachine
  printf("INITIALIZING AbstractMachine\n");
  
  MPI_Init(NULL, NULL);

  // Get the number of processes
  
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  // char node_name[MPI_MAX_PROCESSOR_NAME];
  // int name_len;
  MPI_Get_processor_name(node_name, &name_len);

  return 0;
}