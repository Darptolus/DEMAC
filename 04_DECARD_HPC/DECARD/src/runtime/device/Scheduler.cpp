/**
 * @file Scheduler.cpp
 * @author Diego Roa
 * @date 5 Nov 2021
 * @brief Scheduler
 * @todo Add copyright
 *
 * Includes the definition of Scheduler
 */

#include "SU.hpp"
#include "Node.hpp"

using namespace decard;

void SU_Sch::sort_codelets()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  DECARD_INFOMSG(1, "%s: SU_Sch: Sorting CDs", n_int->node_name);
}

// Codelet * Scheduler::get_CDrdy()
int SU_Sch::get_CDrdy()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  DECARD_INFOMSG(1, "%s: SU_Sch: CD_rdy", n_int->node_name);
  return 0;
}