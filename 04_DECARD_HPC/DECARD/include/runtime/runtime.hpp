/**
 * @file runtime.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Runtime
 * @todo Add copyright
 *
 * Includes the definition of the Runtime
 */

#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "AbstractMachine.hpp"
#include "CodeletGraph.hpp"
#include "Codelet.hpp"

namespace decard
{
  class Runtime
  {
    private:
      CodeletGraph DECARD_CDG;
      AbstractMachine DECARD_CAM;
      Node * t_Node;
      // int world_size;
      // int world_rank;
      // char * node_name;
    public:
      Runtime():
      DECARD_CAM(&DECARD_CDG){
        this->t_Node = DECARD_CAM.get_Node();
      };
      ~Runtime(){};
      int run();
      int end();
      int get_worldsize() {return t_Node->get_wz();};
      int get_worldrank() {return t_Node->get_id();};
      char get_nodename() {
        Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_Node);
        return *(n_int->node_name);};
      Node * get_Node() {return t_Node;};
      //CodeletGraph * get_cdg(){return &DECARD_CDG;);
      void add_TP(ThreadedProcedure * a_TP){
      DECARD_CDG.add_tp(a_TP);
    };
  };
}
#endif /* RUNTIME_H */
