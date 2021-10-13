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
#include <iostream>
#include <omp.h>
#include <mpi.h>
#include <vector>
#include "NodeInterface.hpp"
#include <NCOM.hpp>
#include <NMGR.hpp>
#include <ddarts.hpp>

namespace decard
{
  class Node_Extern: public Node
  {
  private:
    MPI_Request rcv_req;
    MPI_Status rcv_sts;
    int rcv_enb;
    int snd_flg;
    // int test_flagS;
    int rcv_flg;
    int msg_box;
    int msg_out;
  public:
    Node_Extern(int w_rank, int w_size):Node(w_rank, w_size){
      rcv_enb = 0;
    };
    ~Node_Extern(){}
    int run();
    void set_renb(){
      this->rcv_enb = 1;
    }
    void clr_renb(){
      this->rcv_enb = 0;
    }
    void set_sflg(){
      this->snd_flg = 1;
    }
    void clr_sflg(){
      this->snd_flg = 0;
    }
    void set_msgbox(int a_msg){
      this->msg_box = a_msg;
    }
    MPI_Request * get_rreq(){return &rcv_req;}
    MPI_Status * get_rsts(){return &rcv_sts;}
    int * get_rflg(){return &rcv_flg;}
    int * get_msgbox(){return &msg_box;}
    int get_msgout(){return msg_out;}
    int get_renb(){return rcv_enb;}
    int get_sflg(){return snd_flg;}
  };

  class Node_Intern: public Node
  {
  private:
    int exec;
    NCOM this_NCOM;
    NMGR this_NMGR;
    // thread_safe::deque<ThreadedProcedure*> INTPQ;
    // thread_safe::deque<ThreadedProcedure*> ONTPQ;
    // thread_safe::deque<ThreadedProcedure*> ISTPQ;
    // thread_safe::deque<ThreadedProcedure*> OSTPQ;
    cl_q INCLQ; // Input Node Control Queue
    cl_q ONCLQ; // Output Node Control Queue
    cl_q ISCLQ; // Input Scheduler Control Queue
    cl_q OSCLQ; // Output Scheduler Control Queue
    tp_q INTPQ; // Input Node Threaded Procedure Queue
    tp_q ONTPQ; // Output Node Threaded Procedure Queue
    tp_q ISTPQ; // Input Scheduler Threaded Procedure Queue
    tp_q OSTPQ; // Output Scheduler Threaded Procedure Queue
    Node_Extern * node_rcv;
    dDARTS this_dDARTS;
  public: 
    char node_name[HOST_NAME_MAX+1];
    Node_Intern(int w_rank, int w_size, AllNodes * a_nodes):
    Node(w_rank, w_size),
    this_NCOM(a_nodes, this, &INCLQ, &ONCLQ, &INTPQ, &ONTPQ),
    this_NMGR(a_nodes, this, &INCLQ, &ONCLQ, &ISCLQ, &OSCLQ, &INTPQ, &ONTPQ, &ISTPQ, &OSTPQ),
    this_dDARTS(this, &ISCLQ, &OSCLQ, &ISTPQ, &OSTPQ)
    {
      gethostname(node_name, HOST_NAME_MAX+1);
      this->exec = 1;
      node_rcv = NULL;
      // printf("NODENAME: %s \n", node_name);
    };
    ~Node_Intern(){
      // for (auto& element : nodes) {
      //   delete element;
      // }
    };
    void set_exec(){
      this->exec = 1;
    };
    void clr_exec(){
      this->exec = 0;
    };
    void set_nrcv(Node_Extern * a_nrcv){
      this->node_rcv = a_nrcv;
    };
    int get_exec(){return exec;};
    Node_Extern * get_nrcv(){return node_rcv;};
    int run();
  };

} // namespace decard
#endif /* NODE_H */