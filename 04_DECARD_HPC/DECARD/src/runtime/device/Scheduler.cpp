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

void Scheduler::sort_codelets()
{
  DECARD_INFOMSG(1, "%s: Scheduler: Sorting CDs", n_int->node_name);
}

// Codelet * Scheduler::get_CDrdy()
int Scheduler::get_CDrdy()
{
  DECARD_INFOMSG(1, "%s: Scheduler: CD_rdy", n_int->node_name);
  return 1;
}