/**
 * @file Node.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node 
 * @todo Add copyright
 *
 * Includes the definition of the Node for each board of the cluster
 */

#include "Node.hpp"

using namespace decard;

int Node_Intern::start_NODE()
{
  // Initialize NODE
  printf("INITIALIZING Node_Intern\n");

  eDARTS epiDARTS;
  epiDARTS.start_eDARTS();

  // OpenMP
  #pragma omp parallel num_threads(2)
  {
    int core_id = omp_get_thread_num();
    int core_tot = omp_get_num_threads();

    if (core_id == 0){
      // Core A
      NodeNCOM.start_NCOM();
      printf("%s: This is NCOM R(%d/%d) C(%d/%d)\n", this->node_name, this->world_rank+1, this->world_size, core_id+1, core_tot);
    }else if (core_id == 1){
      // Core B
      NodeNMGR.start_NMGR();
      printf("%s: This is NMGR R(%d/%d) C(%d/%d)\n", this->node_name, this->world_rank+1, this->world_size, core_id+1, core_tot);
    }
    #pragma omp barrier
  }
  return 0;
};

int Node_Extern::start_NODE()
{
  // Initialize NODE
  printf("INITIALIZING Node_Extern\n");
  return 0;
};