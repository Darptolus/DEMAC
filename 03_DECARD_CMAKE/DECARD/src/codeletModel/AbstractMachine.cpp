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

// int world_size;
int world_rank;

// AbstractMachine::AbstractMachine(){
//   printf("CREATING AbstractMachine\n");
// }

int AbstractMachine::run()
{
  // Initialize AbstractMachine
  MPI_Init(NULL, NULL);

  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Create Nodes
  Node * new_Node;
  Node * ths_Node;
  for (int i=0; i<world_size; ++i){
    if (i==world_rank){
      // Create Intern Node
      new_Node = new Node_Intern(i, world_size, &nodes);
      ths_Node = new_Node;
      // newNode->start_NODE();
    }else if (i!=world_rank){
      // Create Extern Node
      new_Node = new Node_Extern(i, world_size);
    }
    nodes.push_back(new_Node);
  }
  // Define operation mode for each node
  node_mode n_mode = N_RECEIVE;

  for (nodes_it = nodes.begin(); nodes_it != nodes.end(); ++nodes_it){
    (*nodes_it)->set_mode(n_mode);
  }

  // Setup current node
  // nodes_it = nodes.begin();
  // std::advance(nodes_it, world_rank);
  // (*nodes_it)->run();
  ths_Node->run();


  // TOBEDELETED
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

  return 0;
}

int AbstractMachine::end()
{
  // Finalize the MPI environment. No more MPI calls can be made after this
  // MPI_Finalize();
  return 0;
}