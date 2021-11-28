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

namespace decard
{
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
    C_LWMD  // Low
    C_LWLW  // Low-Low
  };

  class Codelet
  {
  private:
    codelet_status c_status;
    SyncSlot s_slot;
  public:
    Codelet(
      uint32_t a_dep, uint32_t a_res, codelet_status a_status):
      c_status(a_status), s_slot(a_dep, a_res)
      {};
    Codelet(uint32_t dep, uint32_t res):
      s_slot(a_dep, a_res){
        c_status = C_DRMT;
    };
    Codelet(){};
    ~Codelet(){};

    void decDep();
    void resetCodelet();
    void setStatus(codelet_status a_status){
      c_status = a_status;
    };
    codelet_status getStatus(){ return c_status;};
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

  typedef thread_safe::deque<Codelet*> cd_q;
}
#endif /* CODELET_H */