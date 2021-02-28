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
#include <vector>
#include <iterator>
#include "Node.hpp"

namespace decard
{
  enum machine_state
  {
    M_DONE,
    M_EXEC,
    M_IDLE,
    M_UNDEF  
  };
  
  class AbstractMachine
  {
  private:
    int world_size;
    machine_state state;
    AllNodes nodes;
    AllNodes::iterator nodes_it;
  public:
    AbstractMachine(){};
    ~AbstractMachine() {
      for (auto& element : nodes) {
        delete element;
      }
    };
    
    int start_AM();
    void set_state(machine_state a_state){
      this->state = a_state;
    };
    int get_world_size() {return world_size;};
    machine_state get_state() {
      // Update machine state with node states
      
      
      
      return state;};
    
  }; 
}
#endif /* ABSTRACTMACHINE_H */