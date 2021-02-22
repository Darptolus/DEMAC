/**
 * @file runtime.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Runtime
 * @todo Add copyright
 *
 * Includes the definition of the Runtime
 */

#include "runtime.hpp"

// int world_size;
// int world_rank;
// int name_len;
// char node_name[16];

int start_DECARD()
{
  AbstractMachine DECARD_AM;
  // Initialize Runtime
  printf("INITIALIZING RUNTIME\n");
  DECARD_AM.start_AM();
  
  return 0;
}
