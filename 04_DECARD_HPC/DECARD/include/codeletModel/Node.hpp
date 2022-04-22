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
#include "CodeletGraph.hpp"
#include "NCOM.hpp"
#include "NMGR.hpp"
#include "ddarts.hpp"

namespace decard
{
  class Node_Extern: public Node
  {
  private:
    MPI_Request rcv_req;
    MPI_Status rcv_sts;
    int rcv_enb;
    int snd_flg;
    int rcv_flg;
    int msg_id;
    int * msg_box;
    Msg1 msg_in;
    // Msg1 msg_out;
  public:
    Node_Extern(int w_rank, int w_size):Node(w_rank, w_size){
      rcv_enb = 0;
      msg_id = 0;
      msg_in ={0, N_Z, 0, 0}; // ToDo: Replace last zero for NULL ptr
      msg_box = (int *) &msg_in;
    }
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
    void set_msgbox(int * a_msg){
      this->msg_box = a_msg;
    }
    MPI_Request * get_rreq(){return &rcv_req;}
    MPI_Status * get_rsts(){return &rcv_sts;}
    int get_msgid(){return msg_id;}
    int * get_rflg(){return &rcv_flg;}
    Msg1 * get_msgin(){return &msg_in;}
    int * get_msgbox(){return msg_box;}
    int * get_msgtype(){return &msg_box[1];}
    int * get_msgdata(){return &msg_box[3];}
    // int get_msgout(){return msg_out;}
    int get_renb(){return rcv_enb;}
    int get_sflg(){return snd_flg;}
    void inc_msgid(){++msg_id;}
  };

  class Node_Intern: public Node
  {
  private:
    int exec;
    NCOM t_NCOM;
    NMGR t_NMGR;
    CodeletGraph * t_CDG;
    cl_q INCLQ; // Input Node Control Queue
    cl_q ONCLQ; // Output Node Control Queue
    // cl_q ISCLQ; // Input Scheduler Control Queue
    // cl_q OSCLQ; // Output Scheduler Control Queue
    tp_q INTPQ; // Input Node Threaded Procedure Queue
    tp_q ONTPQ; // Output Node Threaded Procedure Queue
    // tp_q ISTPQ; // Input Scheduler Threaded Procedure Queue
    // tp_q OSTPQ; // Output Scheduler Threaded Procedure Queue
    cl_q * t_ISCLQ; // Input Scheduler Control Queue
    cl_q * t_OSCLQ; // Output Scheduler Control Queue
    tp_q * t_ISTPQ; // Input Scheduler Threaded Procedure Queue
    tp_q * t_OSTPQ; // Output Scheduler Threaded Procedure Queue
    Node_Extern * node_rcv; // Receiver Node 
    dDARTS t_dDARTS; // DARTS
    int ncom_idle;
  public: 
    char node_name[HOST_NAME_MAX+1];
    Node_Intern(
      int w_rank, int w_size, AllNodes * a_nodes, CodeletGraph * a_CDG):
      Node(w_rank, w_size), t_CDG(a_CDG),
      t_NCOM(a_nodes, this, &INCLQ, &ONCLQ, &INTPQ, &ONTPQ),
      t_NMGR(a_nodes, this, &t_dDARTS, a_CDG, &INCLQ, &ONCLQ, &INTPQ, &ONTPQ),
      t_dDARTS(this)
      {
        gethostname(node_name, HOST_NAME_MAX+1);
        // printf("NODENAME: %s \n", node_name);
        this->exec = 1;
        node_rcv = NULL;
        ncom_idle = 0;
      }

    // Node_Intern(
    //   int w_rank, int w_size, AllNodes * a_nodes, CodeletGraph * a_CDG):
    //   Node(w_rank, w_size), t_CDG(a_CDG),
    //   t_NCOM(a_nodes, this, &INCLQ, &ONCLQ, &INTPQ, &ONTPQ),
    //   t_NMGR(a_nodes, this, &t_dDARTS, a_CDG, &INCLQ, &ONCLQ, &ISCLQ, &OSCLQ, &INTPQ, &ONTPQ, &ISTPQ, &OSTPQ),
    //   t_dDARTS(this, &ISCLQ, &OSCLQ, &ISTPQ, &OSTPQ)
    //   {
    //     gethostname(node_name, HOST_NAME_MAX+1);
    //     // printf("NODENAME: %s \n", node_name);
    //     this->exec = 1;
    //     node_rcv = NULL;
    //     ncom_idle = 0;
    //   };
    ~Node_Intern(){
      // for (auto& element : nodes) {
      //   delete element;
      // }
    }
    void set_exec(){
      this->exec = 1;
    }
    void set_cidle(){
      this->ncom_idle = 1;
    }
    void clr_exec(){
      this->exec = 0;
    }
    void clr_cidle(){
      this->ncom_idle = 0;
    }
    void set_nrcv(Node_Extern * a_nrcv){
      this->node_rcv = a_nrcv;
    }
    int get_exec(){return exec;}
    int get_cidle(){return ncom_idle;}
    Node_Extern * get_nrcv(){return node_rcv;}
    int run();
  };

} // namespace decard
#endif /* NODE_H */