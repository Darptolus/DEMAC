#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runtime.hpp"
#include "AbstractMachine.hpp"
#include "Node.hpp"

int start_DECARD()
{
  // Initialize Runtime
  printf("INITIALIZING RUNTIME\n");
  start_AM();
  // MPI
  start_NODE();
  return 0;
}
