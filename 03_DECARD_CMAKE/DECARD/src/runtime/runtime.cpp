#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtime.hpp"
#include "AbstractMachine.hpp"
#include "Node.hpp"

// int world_size;
// int world_rank;
// int name_len;
// char node_name[16];

int start_DECARD()
{
  // Initialize Runtime
  printf("INITIALIZING RUNTIME\n");
  start_AM();
  // MPI
  start_NODE();
  return 0;
}
