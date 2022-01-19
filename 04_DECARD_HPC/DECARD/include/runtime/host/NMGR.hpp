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
    nmgr_mode m_mode;
    Node * t_node;
    AllNodes * nodes_list;
    AllNodes::iterator n_it;
    CodeletGraph * t_CDG;
    NMGR_Sch nmgr_sch;  // Node Manager Scheduler
    cl_q * t_INCLQ;           // Input Manager Control Queue
    cl_q * t_ONCLQ;           // Output Manager Control Queue
    cl_q * t_ISCLQ;           // Input Scheduler Control Queue
    cl_q * t_OSCLQ;           // Output Scheduler Control Queue
    tp_q * t_INTPQ;           // Input Node Threaded Procedure Queue
    tp_q * t_ONTPQ;           // Output Node Threaded Procedure Queue
    tp_q * t_ISTPQ;           // Input Scheduler Threaded Procedure Queue
    tp_q * t_OSTPQ;           // Output Scheduler Threaded Procedure Queue
    int maxltp;               // Max Local TP 
    int sptsu;                // Set Point for Scheduler Unit 
  public:
    NMGR( 
      AllNodes * a_nodes, Node * a_node, CodeletGraph * a_CDG,
      cl_q * a_INCLQ, cl_q * a_ONCLQ, cl_q * a_ISCLQ, cl_q * a_OSCLQ,
      tp_q * a_INTPQ, tp_q * a_ONTPQ, tp_q * a_ISTPQ, tp_q * a_OSTPQ):
      nodes_list(a_nodes), t_node(a_node), t_CDG(a_CDG),
      t_INCLQ(a_INCLQ), t_ONCLQ(a_ONCLQ), t_ISCLQ(a_ISCLQ), t_OSCLQ(a_OSCLQ),
      t_INTPQ(a_INTPQ), t_ONTPQ(a_ONTPQ), t_ISTPQ(a_ISTPQ), t_OSTPQ(a_OSTPQ)
      {
        m_mode = M_REMT;
        maxltp = 0; // Max number of local TPS in OSTPQ
      };
    ~NMGR(){};
    int run();
    void set_mode(nmgr_mode a_m_mode){
      this->m_mode = a_m_mode;
    };
    void set_mltp(int a_maxltp){
      this->maxltp = a_maxltp;
    };
    nmgr_mode get_mode(){ return m_mode;};
    int get_mltp(){ return maxltp;};
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
    // int tst_gen();
    int tst_gen_0();
    int tst_gen_1(int x);
  };
}

#endif /* NMGR_H */