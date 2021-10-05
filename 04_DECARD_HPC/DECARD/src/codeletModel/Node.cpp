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

int Node_Intern::run()
{
  // Initialize NODE
  // printf("INITIALIZING Node_Intern\n");

  // Initialize eDARTS
  // epiDARTS.init();
  // epiDARTS.run();
  // usleep(100);

  // OpenMP
  #pragma omp parallel num_threads(2)
  {
    int core_id = omp_get_thread_num();
    int core_tot = omp_get_num_threads();

    if (core_id == 0){
      // Core A
      // printf("%s: NCOM: R(%d/%d) C(%d/%d)\n", this->node_name, this->node_id+1, this->world_size, core_id+1, core_tot);
      this_NCOM.run();
      // this_NCOM.tst_gen_0();
      
    }else if (core_id == 1){
      // Core B
      // printf("%s: NMGR: R(%d/%d) C(%d/%d)\n", this->node_name, this->node_id+1, this->world_size, core_id+1, core_tot);
      this_NMGR.run();
      this_NMGR.tst_gen_1(1);
      d_DARTS.p_res();
    }
    #pragma omp barrier
  }
  // epiDARTS.end();
  return 0;
}

int Node_Extern::run()
{
  // Initialize NODE
  printf("INIT Node_Extern\n");
  return 0;
}