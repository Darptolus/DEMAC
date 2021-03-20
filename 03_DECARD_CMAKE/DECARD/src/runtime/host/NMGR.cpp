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
  
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);

  printf("%s: INITIALIZING NMGR\n", n_int->node_name);
  return 0;
}

// int NMGR::tst_gen()
// {
//   // NMGR  
//   Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
//   Node_Extern * n_ext;
//   printf("%s: NMGR: Generating TPs\n", (n_int)->node_name);
//   ThreadedProcedure* newTP;
//   for(int x = 0; x < 3; x++){
//     for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
//       if ((*n_it)->get_id() != (n_int)->node_id){
//         if ((*n_it)->get_mode(

//         ) == N_RECEIVE){
//           n_ext = dynamic_cast <Node_Extern *> (*n_it);
//           printf("NMGR: NEW TP\n");
//           newTP = new ThreadedProcedure();
//           newTP->set_orig((n_int)->node_id);
//           newTP->set_dest((*n_it)->get_id());
//           newTP->set_opr(((*n_it)->get_id() + 1) * 10 + x);

//           // ONTPQ.push_back(newTP);
//         }
//       }
//     }
//   }
//   return 0;
// }

int NMGR::tst_gen_0()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  ThreadedProcedure * newTP;
  int x, y;
  // Producing
  printf("%s: NMGR: Generating TPs\n", n_int->node_name);
  for(x = 0; x < 3; ++x){
    newTP = new ThreadedProcedure();
    newTP->set_orig(n_int->node_id);
    // newTP->set_dest((*n_it)->get_id());
    newTP->set_opr(x);
    printf("%s: NMGR: Pushing [%d]\n", n_int->node_name, x);
    t_ONTPQ->push_back(newTP);
    // t_OCTRQ->push_back(x);
  }
  usleep(1000000);
  // Consuming
  printf("%s: NMGR: Consuming TPs\n", n_int->node_name);
  x = 0;
  while (x < 3) {
    if(!t_INTPQ->empty()){
      newTP = t_INTPQ->popFront();
      y = newTP->get_opr();
      printf("%s: NMGR: Popping [%d]\n", n_int->node_name, y);
      ++x;
    }
  }
  return 0;
}