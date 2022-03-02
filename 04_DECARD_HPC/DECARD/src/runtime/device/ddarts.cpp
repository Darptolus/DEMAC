/**
 * @file ddarts.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief dDARTS 
 * @todo Add copyright
 *
 * Includes the definition of the accelerator for each board of the cluster
 */
#include "ddarts.hpp"
#include "Node.hpp"

using namespace decard;

int dDARTS::run_SU(){
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  // DECARD_INFOMSG(1, "%s: SU: INIT", n_int->node_name);
  SU.run();
  return 0;
}

int dDARTS::run_CU(){
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  // DECARD_INFOMSG(1, "%s: CU: INIT", n_int->node_name);
  CU.run();
  return 0;
}

void dDARTS::end(){}

// eDARTS
// void eDARTS::end(){
  // e_close(&dev);
  // e_finalize();
// }