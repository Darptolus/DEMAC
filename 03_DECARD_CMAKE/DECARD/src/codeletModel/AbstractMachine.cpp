/**
 * @file AbstractMachine.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Abstract Machine 
 * @todo Add copyright
 *
 * Includes the definition of the Abstract Machine for the cluster
 */
#include "AbstractMachine.hpp"

int world_size;
int world_rank;
// char node_name[16];
// int name_len;

AbstractMachine::AbstractMachine(){
  printf("CREATING AbstractMachine\n");
}

int AbstractMachine::start_AM()
{
  // Initialize AbstractMachine
  printf("INITIALIZING AbstractMachine\n");
  
  MPI_Init(NULL, NULL);

  // Get the number of processes
  
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  // char node_name[MPI_MAX_PROCESSOR_NAME];
  // int name_len;
  // MPI_Get_processor_name(node_name, &name_len);

  // Create Nodes
  for (int i=0; i<world_size; ++i){
    Node * newNode;
    if (i==world_rank){
      // Create Intern Node
      newNode = new Node_Intern(world_rank, world_size);
      // newNode->start_NODE();
    }else if (i!=world_rank){
      // Create Extern Node
      newNode = new Node_Extern(world_rank, world_size);
    }
    nodes.push_back(newNode);
  }

  for (nodes_it = nodes.begin(); nodes_it != nodes.end(); ++nodes_it){
    (*nodes_it)->start_NODE();
  }

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

