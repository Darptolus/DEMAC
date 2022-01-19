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
  class NMGR_Sch
  {
  private:
    tp_q * t_TPQ; // Codelet Queue
    Node * t_node; // This node
  public:
    NMGR_Sch(
      tp_q * a_TPQ):
      t_TPQ(a_TPQ){};
    NMGR_Sch(){};
    ~NMGR_Sch(){};
    void setTPQ(tp_q * a_TPQ){
      this->t_TPQ = a_TPQ;
    };
    // void sort_codelets();
    // // Codelet * get_CDrdy ();
    // int getTP ();
    // int get_CDnum ();
  };

  class Scheduler
  {
  private:
    cd_q * t_CDQ; // Codelet Queue
    Node * t_node; // This node
  public:
    Scheduler(
      cd_q * a_CDQ):
      t_CDQ(a_CDQ){};
    Scheduler(){};
    ~Scheduler(){};
    void setCDQ(cd_q * a_CDQ){
      this->t_CDQ = a_CDQ;
    };
    void sort_codelets();
    // Codelet * get_CDrdy ();
    int get_CDrdy ();
    int get_CDnum ();
  };
}

#endif /* SCHEDULER_H */