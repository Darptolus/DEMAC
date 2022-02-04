/**
 * @file NodeInterface.hpp
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
#include <mpi.h>
#include "thread_safe_deque.h"
#include "CodeletGraph.hpp"
// #include "ThreadedProcedure.hpp"
#include "Operations.hpp"
#include "Messages.hpp"
#include <vector>

namespace decard
{
  enum node_mode
  {
    N_DONE, // Node Done
    N_IDLE, // Node Idle
    N_RECV, // Node Receive
    N_SEND  // Node Send
  };
  
  enum node_type
  {
    N_INTERN,
    N_EXTERN
  };

  class Node
  {
  private:
  public:
    int node_id;
    int world_size;
    node_type n_type;
    node_mode n_mode;
    Node(
      int a_rank, int a_size):
      node_id(a_rank), world_size(a_size){};
    ~Node(){};
    virtual int run() = 0; // Pure virtual
    void set_mode(node_mode a_n_mode){
      this->n_mode = a_n_mode;
    };
    void set_type(node_type a_n_type){
      this->n_type = a_n_type;
    };
    int get_id(){return node_id;};
    node_mode get_mode(){return n_mode;};
    node_type get_type(){return n_type;};
    void mode_dne(){ // Set mode DONE
      this->n_mode = N_DONE;
    };
    void mode_idl(){ // Set mode IDLE
      this->n_mode = N_IDLE;
    };
    void mode_rcv(){ // Set mode RECV
      this->n_mode = N_RECV;
    };
    void mode_snd(){ // Set mode SEND
      this->n_mode = N_SEND;
    };
  };

  typedef std::vector<Node*> AllNodes;
  // typedef thread_safe::deque<Message*> cl_q;
  // typedef thread_safe::deque<ThreadedProcedure*> tp_q;
  // typedef thread_safe::deque<Codelet*> cd_q;
} // namespace decard
#endif /* NODEINIT_H */