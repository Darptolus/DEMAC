#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#include "Node.hpp"

int start_NODE()
{
  // Initialize NODE
  printf("INITIALIZING NODE\n");
  start_eDARTS();

  // OpenMP
  #pragma omp parallel num_threads(2)
  {
    int core_id = omp_get_thread_num();
    int core_tot = omp_get_num_threads();

    if (core_id == 0){
      // Core A
      start_NCOM();
    }else if (core_id == 1){
      // Core B
      start_NMGR();
    }
  }
  return 0;
}
