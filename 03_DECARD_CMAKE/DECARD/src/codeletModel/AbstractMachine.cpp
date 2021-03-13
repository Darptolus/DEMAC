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

// void print_rt_mode(machine_state mstate) {
//   printf("RUNTIME ");
//   switch(mstate) {
//     case ALL_AUTO:
//         printf("ALL_AUTO\n");
//         break;
//     case ALL_DONE:
//         printf("ALL_DONE\n");
//         break;
//     case ALL_IDLE:
//         printf("ALL_IDLE\n");
//         break;
//     case ALL_RECV:
//         printf("ALL_RECV\n");
//         break;
//     case ALL_SEND:
//         printf("ALL_SEND\n");
//         break;
//     default:
//         printf("Invalid Mode\n");
//   }
// }

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
  for (int i=0; i<world_size; ++i){
    Node * newNode;
    if (i==world_rank){
      // Create Intern Node
      newNode = new Node_Intern(i, world_size, &nodes);
      // newNode->start_NODE();
    }else if (i!=world_rank){
      // Create Extern Node
      newNode = new Node_Extern(i, world_size);
    }
    nodes.push_back(newNode);
  }
  // Define operation mode for each node
  node_mode n_mode = N_IDLE;
  // switch(am_state) {
  //   case ALL_AUTO:
  //       // Set mode based on workload for each node?
  //       n_mode = N_IDLE;
  //       break;
  //   case ALL_DONE:
  //       n_mode = N_DONE;
  //       break;
  //   case ALL_IDLE:
  //       n_mode = N_IDLE;
  //       break;
  //   case ALL_RECV:
  //       n_mode = N_RECEIVE;
  //       break;
  //   case ALL_SEND:
  //       n_mode = N_SEND;
  //       break;
  //   default:
  //       printf("Invalid Mode\n");
  // }
  for (nodes_it = nodes.begin(); nodes_it != nodes.end(); ++nodes_it){
    (*nodes_it)->set_mode(n_mode);
  }

  // Setup current node
  nodes_it = nodes.begin();
  std::advance(nodes_it, world_rank);
  (*nodes_it)->run();



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

