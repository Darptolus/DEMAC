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
#include <NCOM.hpp>
#include <NMGR.hpp>
#include <eDARTS.hpp>

class Node
{
private:
public:
  int world_rank;
  int world_size;
  Node(int w_rank, int w_size):world_rank(w_rank),world_size(w_size){};
  ~Node(){};
  virtual int start_NODE() = 0;
};

class Node_Intern: public Node
{
private:
public:
 char node_name[HOST_NAME_MAX+1];
  Node_Intern(int w_rank, int w_size):Node(w_rank, w_size){
    gethostname(node_name, HOST_NAME_MAX+1);
    // printf("NODENAME: %s \n", node_name);
  };
  ~Node_Intern(){};
  int start_NODE();
};

class Node_Extern: public Node
{
private:
public:
  Node_Extern(int w_rank, int w_size):Node(w_rank, w_size){};
  ~Node_Extern(){};
  int start_NODE();
};

#endif /* NODE_H */