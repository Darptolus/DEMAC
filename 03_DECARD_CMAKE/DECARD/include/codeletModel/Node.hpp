/**
 * @file Node.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node 
 * @todo Add copyright
 *
 * Includes the definition of the Node for each board of the cluster
 */

#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <omp.h>
#include "NodeInit.hpp"
#include <NCOM.hpp>
#include <NMGR.hpp>
#include <vector>
#include <edarts.hpp>

namespace decard
{
  class Node_Intern: public Node
  {
  private:
    NCOM NodeNCOM;
    NMGR NodeNMGR;
  public:
  char node_name[HOST_NAME_MAX+1];
    Node_Intern(int w_rank, int w_size, AllNodes * a_nodes):Node(w_rank, w_size), NodeNCOM(a_nodes, this){
      gethostname(node_name, HOST_NAME_MAX+1);
      // printf("NODENAME: %s \n", node_name);
    };
    ~Node_Intern(){};
    int start_NODE();
  };

  class Node_Extern: public Node
  {
  private:
    // ACK

  public:
    Node_Extern(int w_rank, int w_size):Node(w_rank, w_size){};
    ~Node_Extern(){};
    int start_NODE();
  };
} // namespace decard
#endif /* NODE_H */