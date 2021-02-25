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
  enum comms_mode
  {
    INIT,
    RECEIVE,
    SEND
  };

  class Node
  {
  private:
  public:
    int world_rank;
    int world_size;
    comms_mode comm_mode;
    thread_safe::deque<ThreadedProcedure*> INTPQ;
    thread_safe::deque<ThreadedProcedure*> ONTPQ;
    thread_safe::deque<ThreadedProcedure*> ISTPQ;
    thread_safe::deque<ThreadedProcedure*> OSTPQ;

    Node(int w_rank, int w_size):world_rank(w_rank),world_size(w_size){};
    ~Node(){};
    virtual int start_NODE() = 0;
  };

  typedef std::vector<Node*> AllNodes;
} // namespace decard
#endif /* NODEINIT_H */