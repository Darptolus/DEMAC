/**
 * @file NCOM.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Comunicator
 * @todo Add copyright
 *
 * Includes the definition of the Node Comunicator for each node of the cluster
 */

#ifndef NCOM_H
#define NCOM_H

#include <stdio.h>
#include <stdlib.h>
#include "NodeInterface.hpp"
#include "DECARD_tools.hpp"
#include <mpi.h>
#include <vector>
#include <iterator>

namespace decard
{
  class NCOM
  {
  private:
    Node * t_node;
    // Node * n_recv;
    AllNodes * nodes_list;
    AllNodes::iterator n_it;
    cl_q * t_INCLQ; // Input Node Control Queue
    cl_q * t_ONCLQ; // Output Node Control Queue
    tp_q * t_INTPQ; // Input Node Control Queue
    tp_q * t_ONTPQ; // Output Node Control Queue
    int ErrCode;
  public:
    NCOM( AllNodes * a_nodes, Node * a_node, 
          cl_q * a_INCLQ, cl_q * a_ONCLQ,
          tp_q * a_INTPQ, tp_q * a_ONTPQ):
          nodes_list(a_nodes), t_node(a_node), 
          t_INCLQ(a_INCLQ), t_ONCLQ(a_ONCLQ),
          t_INTPQ(a_INTPQ), t_ONTPQ(a_ONTPQ)
          {};
    ~NCOM(){};
    int run();
    int tst_gen_0();
    // int tst_gen_1();
  };
}

#endif /* NCOM_H */
