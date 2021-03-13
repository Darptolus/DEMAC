/**
 * @file NMGR.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Manager
 * @todo Add copyright
 *
 * Includes the definition of the Node Manager for each node of the cluster
 */

#include "NMGR.hpp"
#include "Node.hpp"

using namespace decard;

int NMGR::run()
{
  // NMGR initialize Accelerator
  printf("INITIALIZING NMGR\n");
  return 0;
}

int NMGR::tst_gen()
{
  // NMGR  
  Node_Intern * t_node = (Node_Intern *) this_node;
  Node_Extern * n_ext;
  printf("NMGR: Generating TPs\n");
  ThreadedProcedure* newTP;
  for(int x = 0; x < 3; x++)
  for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
    if ((*n_it)->get_id() != (t_node)->node_id){
      if ((*n_it)->get_mode() == N_RECEIVE){
        n_ext = dynamic_cast <Node_Extern *> (*n_it);
        printf("NMGR: NEW TP\n");
        newTP = new ThreadedProcedure();
        newTP->set_orig((t_node)->node_id);
        newTP->set_dest((*n_it)->get_id());
        newTP->set_opr(((*n_it)->get_id() + 1) * 10 + x);
        // ONTPQ.push_back(newTP);
      }
    }
  }


  return 0;
}