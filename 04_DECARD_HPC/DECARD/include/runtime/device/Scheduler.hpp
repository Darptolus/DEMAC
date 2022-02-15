/**
 * @file Scheduler.hpp
 * @author Diego Roa
 * @date 5 Nov 2021
 * @brief Scheduler
 * @todo Add copyright
 *
 * Includes the definition of Scheduler
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include "NodeInterface.hpp"
#include "Codelet.hpp"

namespace decard
{
  // class NMGR_Sch
  // {
  // private:
  //   Node * t_node; // This node
  //   tp_q * t_TPQ; // Codelet Queue
  //   Codelet * newCD;
  // public:
  //   NMGR_Sch(
  //     tp_q * a_TPQ):
  //     t_TPQ(a_TPQ){};
  //   NMGR_Sch(){};
  //   ~NMGR_Sch(){};
  //   void setTPQ(tp_q * a_TPQ){
  //     this->t_TPQ = a_TPQ;
  //   };
  //   // void sort_codelets();
  //   // // Codelet * get_CDrdy ();
  //   // int getTP ();
  //   // int get_CDnum ();
  // };

  class SU_Sch
  {
  private:
    cd_q * t_CDQ; // Codelet Queue
    tp_q * t_TPQ; // Codelet Queue
    tp_q::iterator tps_it; // Iterator
    Node * t_node; // This node
    cd_v * t_cds; // TP's Codelets
    cd_v::iterator cds_it; // Iterator
  public:
    SU_Sch(
      Node * a_node,
      tp_q * a_TPQ):
      t_node(a_node),
      t_TPQ(a_TPQ){};
    // SU_Sch(){};
    ~SU_Sch(){};
    void setCDQ(cd_q * a_CDQ){
      this->t_CDQ = a_CDQ;
    };
    void sort_codelets();
    int cd_rdy();
    Codelet * get_rdy();
    // Codelet * get_CDrdy ();
    // int get_CDrdy ();
  };
}

#endif /* SCHEDULER_H */