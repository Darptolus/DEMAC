/**
 * @file NodeInit.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Initialization
 * @todo Add copyright
 *
 * Includes the definition of the Node for each board of the cluster
 */

#ifndef NODEINIT_H
#define NODEINIT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include "thread_safe_deque.h"
#include "ThreadedProcedure.hpp"
#include <vector>

namespace decard
{
  enum node_mode
  {
    N_DONE,
    N_IDLE,
    N_RECEIVE,
    N_SEND  
  };

  class Node
  {
  private:
  public:
    int world_rank;
    int world_size;
    node_mode comm_mode;
    thread_safe::deque<ThreadedProcedure*> INTPQ;
    thread_safe::deque<ThreadedProcedure*> ONTPQ;
    thread_safe::deque<ThreadedProcedure*> ISTPQ;
    thread_safe::deque<ThreadedProcedure*> OSTPQ;

    Node(int w_rank, int w_size):world_rank(w_rank),world_size(w_size){};
    ~Node(){};
    virtual int start_NODE() = 0; // Pure virtual
    void set_mode(node_mode a_c_mode){
      this->comm_mode = a_c_mode;
    };
  };

  typedef std::vector<Node*> AllNodes;
} // namespace decard
#endif /* NODEINIT_H */