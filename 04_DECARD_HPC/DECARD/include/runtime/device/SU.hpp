/**
 * @file SU.hpp
 * @author Diego Roa
 * @date 15 Oct 2021
 * @brief SU
 * @todo Add copyright
 *
 * Includes the definition of SU
 */

#ifndef SU_H
#define SU_H

#include <stdio.h>
#include <stdlib.h>
#include "NodeInterface.hpp"
#include "Codelet.hpp"
#include "CU.hpp"
#include "Scheduler.hpp"

namespace decard
{
  enum su_mode
  {
    S_DONE, // Scheduling Unit Done
    S_IDLE, // Scheduling Unit Idle
    // S_INTP, // Scheduling Unit Init TP (?)
    S_PSCD, // Scheduling Unit Push Codelet
    S_REMT  // Scheduling Unit Switch TP Remote
  };

  class SU
  {
  private:
    su_mode s_mode; // Scheduler mode
    Node * t_node;  // This node
    CU * t_CU; // CU // ToDo: Add multiple CUs
    SU_Sch sch;  // Scheduler for SU
    cl_q * t_ISCLQ; // Input Scheduler Control Queue
    cl_q * t_OSCLQ; // Output Scheduler Control Queue
    tp_q * t_ISTPQ; // Input Scheduler Threaded Procedure Queue
    tp_q * t_OSTPQ; // Output Scheduler Threaded Procedure Queue
    // Scheduler
    cd_v * t_cds; // TP's Codelets
    cd_q * t_CDQ; // Codelet Queue
    cd_v::iterator cds_it; // Iterator
    tp_q::iterator tps_it; // Iterator
    int max_istpq; // Max number of TPs in ISTPQ
    int min_istpq; // Min number of TPs in ISTPQ
  public:
    SU(
      Node * a_node, CU * a_CU, 
      cl_q * a_ISCLQ, cl_q * a_OSCLQ,
      tp_q * a_ISTPQ, tp_q * a_OSTPQ):
      t_node(a_node), t_CU(a_CU),
      sch(a_node, a_ISTPQ),
      t_ISCLQ(a_ISCLQ), t_OSCLQ(a_OSCLQ),
      t_ISTPQ(a_ISTPQ), t_OSTPQ(a_OSTPQ)
      {
        s_mode = S_IDLE;
        max_istpq = 10;
        min_istpq = 0;
      };
    ~SU(){};
    int run();
    // void addCU(CU * a_CU){};
    void set_mode(su_mode a_su_mode){ this->s_mode = a_su_mode;};
    // void set_mstp(int a_maxstp){
    //   this->maxstp = a_maxstp;
    // };
    su_mode get_mode(){ return s_mode;};
    // int get_mstp(){ return maxstp;};
    void mode_dne(){ this->s_mode = S_DONE;};
    void mode_idl(){ this->s_mode = S_IDLE;};
    // void mode_itp(){ this->s_mode = S_INTP;};
    void mode_pcd(){ this->s_mode = S_PSCD;};
    void mode_rmt(){ this->s_mode = S_REMT;};
  };
}

#endif /* SU_H */