/**
 * @file AbstractMachine.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Abstrac tMachine
 * @todo Add copyright
 *
 * Includes the definition of the Abstract Machine for the runtime
 */

#ifndef ABSTRACTMACHINE_H
#define ABSTRACTMACHINE_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <vector>
#include <iterator>
#include "Node.hpp"
#include "CodeletGraph.hpp"

namespace decard
{
  class AbstractMachine
  {
  private:
    int world_size;
    int world_rank;
    Node * t_Node;
    AllNodes nodes;
    AllNodes::iterator nodes_it;
    CodeletGraph * decard_CDG;
  public:
    AbstractMachine(
      CodeletGraph * a_CDG):
      decard_CDG(a_CDG){
      // Initialize AbstractMachine
      MPI_Init(NULL, NULL);

      // Get the number of processes
      MPI_Comm_size(MPI_COMM_WORLD, &world_size);

      // Get the rank of the process
      MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

      // Create Nodes
      Node * new_Node;
      for (int i=0; i<world_size; ++i){
        if (i==world_rank){
          // Create Intern Node
          new_Node = new Node_Intern(i, world_size, &nodes, decard_CDG);
          t_Node = new_Node;
          // newNode->start_NODE();
        }else if (i!=world_rank){
          // Create Extern Node
          new_Node = new Node_Extern(i, world_size);
        }
        nodes.push_back(new_Node);
      }
      // Define operation mode for each node
      node_mode n_mode = N_RECV;

      for (nodes_it = nodes.begin(); nodes_it != nodes.end(); ++nodes_it){
        (*nodes_it)->set_mode(n_mode);
      }
    };
    ~AbstractMachine() {
      for (auto& element : nodes) {
        delete element;
      }
    };
    int run();
    void set_CDG(CodeletGraph * a_CDG){
      this->decard_CDG = a_CDG;};
    int get_worldsize() {return world_size;};
    int get_worldrank() {return world_rank;};
    Node * get_Node() {return t_Node;};
    CodeletGraph * get_CDG() {return decard_CDG;};
    int end();
  }; 
}
#endif /* ABSTRACTMACHINE_H */