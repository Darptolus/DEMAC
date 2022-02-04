/**
 * @file Codelet.hpp
 * @author Diego Roa
 * @date 14 Oct 2021
 * @brief Codelet
 * @todo Add copyright
 *
 * Includes the definition of codelets
 */

#ifndef CODELET_H
#define CODELET_H

#include <stdlib.h>
#include <SyncSlot.hpp>
#include "thread_safe_deque.h"
namespace decard
{
  //This is a forward declaration since there is a circular dependence
  class ThreadedProcedure;  

  enum codelet_status
  {
    C_DRMT, // Codelet Dormant
    C_ENBL, // Codelet Enable
    C_FIRE, // Codelet Fire
    C_ACTV  // Codelet Active
  };

  enum codelet_priority
  {
    P_HIHI, // High-High
    C_HIMD, // High
    C_MDMD, // Medium
    C_LWMD,  // Low
    C_LWLW  // Low-Low
  };

  class Codelet
  {
  private:
    ThreadedProcedure * t_TP;
    SyncSlot s_slot;
    codelet_status c_status;
  public:
    // TP, # Dependences, # Reset, Status
    Codelet(
      ThreadedProcedure * a_TP, 
      codelet_status a_status,
      uint32_t a_dep, uint32_t a_res):
      t_TP(a_TP),
      c_status(a_status)
      {
        s_slot.initSS(a_dep, a_res);
      };
    // TP, # Dependences, # Reset
    Codelet(
      ThreadedProcedure * a_TP,
      uint32_t a_dep, uint32_t a_res): 
      t_TP(a_TP){
        c_status = C_DRMT;
        s_slot.initSS(a_dep, a_res);
      };
    // TP, # Dependences
    Codelet(
      ThreadedProcedure * a_TP,
      uint32_t a_dep): 
      t_TP(a_TP){
        c_status = C_DRMT;
        s_slot.initSS(a_dep, a_dep);
    };
    Codelet(){};
    ~Codelet(){};
    void decDep();
    void initCodelet(uint32_t a_dep, uint32_t a_res, ThreadedProcedure * a_TP, codelet_status a_status){
      s_slot.initSS(a_dep, a_dep);
      c_status = a_status;
      t_TP = a_TP;
    };
    void resetCodelet(){
      s_slot.resetCounter();
    };
    void setStatus(codelet_status a_status){
      c_status = a_status;
    };
    void setTP(ThreadedProcedure * a_TP){
      t_TP = a_TP;
    };
    codelet_status getStatus(){ return c_status;};
    ThreadedProcedure * getTP(){ return t_TP;};
    uint32_t getDep(){ return s_slot.getCounter();};
    SyncSlot * getSyncSlot();
    virtual void fire(void) = 0; // Codelet firing rule
    void stus_drmt(){ // Set status DRMT
      this->c_status = C_DRMT;
    };
    void stus_enbl(){ // Set status ENBL
      this->c_status = C_ENBL;
    };
    void stus_fire(){ // Set status FIRE
      this->c_status = C_FIRE;
    };
    void stus_actv(){ // Set status ACTV
      this->c_status = C_ACTV;
    };
  };

  // Codelet Queue
  typedef thread_safe::deque<Codelet*> cd_q;
}
#endif /* CODELET_H */