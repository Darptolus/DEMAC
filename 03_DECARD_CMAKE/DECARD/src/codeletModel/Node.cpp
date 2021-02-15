#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#include "Node.hpp"

int world_size;
int world_rank;
int name_len;
char node_name[16];

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
      printf("%s: This is NCOM R(%d/%d) C(%d/%d)\n", node_name, world_rank+1, world_size, core_id+1, core_tot);
    }else if (core_id == 1){
      // Core B
      start_NMGR();
      printf("%s: This is NMGR R(%d/%d) C(%d/%d)\n", node_name, world_rank+1, world_size, core_id+1, core_tot);
    }
  }
  return 0;
}
