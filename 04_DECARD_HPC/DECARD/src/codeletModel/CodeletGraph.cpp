/**
 * @file AbstractMachine.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Abstract Machine 
 * @todo Add copyright
 *
 * Includes the definition of the Abstract Machine for the cluster
 */

#include "CodeletGraph.hpp"
#include "Node.hpp"

using namespace decard;

// void CodeletGraph::get_tps(Node * a_node, tp_q * a_tpq)
void CodeletGraph::get_tps(Node * a_node)
{
// Assigns TPs to specific node TPQ
 Node_Intern * n_int = dynamic_cast <Node_Intern *> (a_node);
//   for (tps_it = all_tps.begin(); tps_it != all_tps.end(); ++tps_it){
//     if((*tps_it)->get_orig_id() == n_int->get_id()){
//       DECARD_INFOMSG(1, "%s: TP assigned to INTPQ", n_int->node_name);
//       n_int->pushto_INTPQ(*tps_it);
//     }
//   }
}
