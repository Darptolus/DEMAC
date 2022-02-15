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
int SU_Sch::cd_rdy()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  for (tps_it = t_TPQ->begin(); tps_it != t_TPQ->end(); ++tps_it){
    // Get TP's Codelet Queue
    t_cds = (*tps_it)->get_cdv();
    for (cds_it = t_cds->begin(); cds_it != t_cds->end(); ++cds_it){
      if ((*cds_it)->get_status() == C_ENBL){
        DECARD_INFOMSG(1, "%s: SU_Sch: CD_rdy CD_%03d", n_int->node_name, (*cds_it)->get_id());
        // Return Codelet
      }
    }
  }
  return 0;
}

Codelet * SU_Sch::get_rdy()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  DECARD_INFOMSG(1, "%s: SU_Sch: CD_rdy", n_int->node_name);
  return 0;
}