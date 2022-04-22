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
  enum ncom_mode
  {
    C_IDLE, // Communicator Idle
    C_RECV, // Communicator Receive
    C_SEND  // Communicator Send
  };
  class NCOM
  {
  private:
    ncom_mode c_mode; // Node Communicator Mode
    Node * t_node;  // This Node
    // Node * n_recv;
    AllNodes * nodes_list;
    AllNodes::iterator n_it;
    CodeletGraph * t_CDG; // Codelet Graph 
    ThreadedProcedure * newTP;
    cl_q * t_INCLQ; // Input Node Control Queue
    cl_q * t_ONCLQ; // Output Node Control Queue
    tp_q * t_INTPQ; // Input Node Control Queue
    tp_q * t_ONTPQ; // Output Node Control Queue
    int ErrCode;
    // int msg_id;
    int msg_size;
    int * p_msg;
    Msg1 msg_1;
    Msg1 * pmsg_1;
    // Msg2 msg_2;
  public:
    NCOM( 
      AllNodes * a_nodes, Node * a_node, 
      cl_q * a_INCLQ, cl_q * a_ONCLQ,
      tp_q * a_INTPQ, tp_q * a_ONTPQ):
      nodes_list(a_nodes), t_node(a_node), 
      t_INCLQ(a_INCLQ), t_ONCLQ(a_ONCLQ),
      t_INTPQ(a_INTPQ), t_ONTPQ(a_ONTPQ)
      {
        c_mode = C_RECV;
      };
    ~NCOM(){};
    int run();
    void set_mode(ncom_mode a_c_mode){
      this->c_mode = a_c_mode;
    };    
    ncom_mode get_mode(){ return c_mode;};
    // Set mode IDLE
    void mode_idl(){ this->c_mode = C_IDLE;};
    // Set mode RECV
    void mode_rcv(){ this->c_mode = C_RECV;};
    // Set mode SEND
    void mode_snd(){ this->c_mode = C_SEND;};
    // int tst_gen_0();
    // int tst_gen_1();
  };
}

#endif /* NCOM_H */
