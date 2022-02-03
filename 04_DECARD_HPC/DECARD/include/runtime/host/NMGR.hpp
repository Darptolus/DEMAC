/**
 * @file NMGR.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Manager
 * @todo Add copyright
 *
 * Includes the definition of the Node Manager for each node of the cluster
 */

#ifndef NMGR_H
#define NMGR_H

#include <stdio.h>
#include <stdlib.h>
#include "NodeInterface.hpp"
#include "CodeletGraph.hpp"
#include "Scheduler.hpp"
#include "ddarts.hpp"

namespace decard
{
  enum nmgr_mode
  {
    M_DONE, // Manager Done
    M_IDLE, // Manager Idle
    M_LOCL, // Manager Local
    M_REMT  // Manager Remote
  };

  class NMGR
  {
  private:
    nmgr_mode m_mode; // Node Manager Mode
    Node * t_node; // This Node
    dDARTS * t_dDARTS; // DARTS
    AllNodes * nodes_list;
    AllNodes::iterator n_it;
    CodeletGraph * t_CDG; // Codelet Graph 
    tp_q * all_tps;
    tp_q::iterator tps_it;
    NMGR_Sch nmgr_sch;  // Node Manager Scheduler
    cl_q * t_INCLQ; // Input Manager Control Queue
    cl_q * t_ONCLQ; // Output Manager Control Queue
    cl_q * t_ISCLQ; // Input Scheduler Control Queue
    cl_q * t_OSCLQ; // Output Scheduler Control Queue
    tp_q * t_INTPQ; // Input Node Threaded Procedure Queue
    tp_q * t_ONTPQ; // Output Node Threaded Procedure Queue
    tp_q * t_ISTPQ; // Input Scheduler Threaded Procedure Queue
    tp_q * t_OSTPQ; // Output Scheduler Threaded Procedure Queue
    int max_ostpq; // Max number of TPS in OSTPQ 
    int max_istpq; // Max number of TPs in ISTPQ
  public:
    NMGR(
      AllNodes * a_nodes, Node * a_node, dDARTS * a_dDARTS, CodeletGraph * a_CDG,
      cl_q * a_INCLQ, cl_q * a_ONCLQ, cl_q * a_ISCLQ, cl_q * a_OSCLQ,
      tp_q * a_INTPQ, tp_q * a_ONTPQ, tp_q * a_ISTPQ, tp_q * a_OSTPQ):
      nodes_list(a_nodes), t_node(a_node), t_dDARTS(a_dDARTS), t_CDG(a_CDG),
      t_INCLQ(a_INCLQ), t_ONCLQ(a_ONCLQ), t_ISCLQ(a_ISCLQ), t_OSCLQ(a_OSCLQ),
      t_INTPQ(a_INTPQ), t_ONTPQ(a_ONTPQ), t_ISTPQ(a_ISTPQ), t_OSTPQ(a_OSTPQ)
      {
        m_mode = M_IDLE;
        all_tps = t_CDG->get_atps();
        max_ostpq = 0; // Max number of TPS in OSTPQ
        max_istpq = 10; // Max number of TPs in ISTPQ
      };
    ~NMGR(){};
    int get_tps();
    int run();
    void set_mode(nmgr_mode a_m_mode){
      this->m_mode = a_m_mode;
    };
    void set_mx_ostpq(int a_max_ostpq){
      this->max_ostpq = a_max_ostpq;
    };
    nmgr_mode get_mode(){return m_mode;};
    int get_mx_ostpq(){return max_ostpq;};
    void mode_dne(){
      this->m_mode = M_DONE;
    };
    void mode_idl(){
      this->m_mode = M_IDLE;
    };
    void mode_lcl(){
      this->m_mode = M_LOCL;
    };
    void mode_rmt(){
      this->m_mode = M_REMT;
    };
    int all_empty(){
      // Check if all Queues are empty
      int empty = 0;
      if (t_INCLQ->empty() && t_ONCLQ->empty() && t_ISCLQ->empty() && t_OSCLQ->empty() &&
          t_INTPQ->empty() && t_ONTPQ->empty() && t_OSTPQ->empty() && t_ISTPQ->empty()){
            empty = 1; // All queues empty
      }
      return empty;
    };
    int all_done();
    // int tst_gen();
    // int tst_gen_0();
    // int tst_gen_1(int x);
  };
}

#endif /* NMGR_H */