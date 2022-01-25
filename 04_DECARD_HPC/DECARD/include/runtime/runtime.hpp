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
      AbstractMachine DECARD_CAM;
      CodeletGraph DECARD_CDG;
      Node * t_Node;
    public:
      char node_name[HOST_NAME_MAX+1];
      Runtime():
      DECARD_CAM(&DECARD_CDG){
        gethostname(node_name, HOST_NAME_MAX+1);
      };;
      ~Runtime(){};
      int run();
      int end();
      int get_worldsize() {return DECARD_CAM.get_worldsize();};
      int get_worldrank() {return DECARD_CAM.get_worldrank();};
      Node * get_Node() {return t_Node;};
  };
}
#endif /* RUNTIME_H */