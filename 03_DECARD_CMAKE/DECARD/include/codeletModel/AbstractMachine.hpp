/**
 * @file AbstractMachine.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Abstrac tMachine
 * @todo Add copyright
 *
 * Includes the definition of the Abstract Machine for the cluster
 */

#ifndef ABSTRACTMACHINE_H
#define ABSTRACTMACHINE_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iterator>
#include <vector>

#include "Node.hpp"

namespace decard
{
  class AbstractMachine
  {
  private:
    int world_size;
    AllNodes nodes;
    AllNodes::iterator nodes_it;
  public:
    AbstractMachine();
    int GetWorldSize() {return world_size;}
    int start_AM();
    ~AbstractMachine() {
      for (auto& element : nodes) {
        delete element;
      }
    }
    
  }; 
}
#endif /* ABSTRACTMACHINE_H */