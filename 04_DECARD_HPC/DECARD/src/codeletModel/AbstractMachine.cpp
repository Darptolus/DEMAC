/**
 * @file AbstractMachine.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Abstract Machine 
 * @todo Add copyright
 *
 * Includes the definition of the Abstract Machine for the cluster
 */
#include "runtime.hpp"
#include "AbstractMachine.hpp"

using namespace decard;

int AbstractMachine::run()
{
  // Run current node
  t_Node->run();

  return 0;
}

int AbstractMachine::end()
{
  // Finalize the MPI environment. No more MPI calls can be made after this
  // MPI_Finalize();
  return 0;
}

// TOBEDELETED

// int world_size;


// AbstractMachine::AbstractMachine(){
//   printf("CREATING AbstractMachine\n");
// }

  // Node * local_Node; = nodes_it;
  // Node_Intern * localNode = (Node_Intern *) local_Node;
  // (*localNode)->start_NODE();

  // for (nodes_it = nodes.begin(); nodes_it != nodes.end(); ++nodes_it){
  //   (*nodes_it)->start_NODE();
  // }
  

  // for (.... num Node) {
  //   Node * current;
  //   if()

  //   current = (new Node_extern(world_size))

  //   current = (new Node_Intern(world_size));

  //   current->start_NODE();
  //   nodes.push_back(current);
  //   // Iteradores
  //   std::vector<Node*>::iterator it;
  //   for (auto it = nodes.begin(); it != nodes.end(); it++)

  //   for (auto& element : nodes) {
      
  //   }
  //   nodes.rbegin()->start_Node();
  // }
  // Node_Exter::emplace
