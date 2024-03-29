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
#include <vector>
#include "Atomics.h"

namespace decard
{
  //This is a forward declaration since there is a circular dependence
  class ThreadedProcedure;
  class ComputationalUnit;

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
    C_LWMD, // Low
    C_LWLW  // Low-Low
  };

  class Codelet
  {
  private:
    ThreadedProcedure * t_TP;
    SyncSlot s_slot;
    codelet_status c_status;
    codelet_priority c_priority;
    ComputationalUnit * t_CU;
    int cd_id;
    int n_exec; // Number of executions -> Default = 1
  public:
    // TP, # Dependences, # Reset, Status
    Codelet(
      ThreadedProcedure * a_TP, 
      codelet_status a_status,
      uint32_t a_dep, uint32_t a_res):
      t_TP(a_TP),
      c_status(a_status)
      {
        s_slot.init_SS(a_dep, a_res);
        n_exec = 1;
      }
    // TP, # Dependences, # Reset
    Codelet(
      ThreadedProcedure * a_TP,
      uint32_t a_dep, uint32_t a_res): 
      t_TP(a_TP){
        c_status = C_DRMT;
        s_slot.init_SS(a_dep, a_res);
        n_exec = 1;
      }
    // TP, # Dependences
    Codelet(
      ThreadedProcedure * a_TP,
      uint32_t a_dep): 
      t_TP(a_TP){
        c_status = C_DRMT;
        s_slot.init_SS(a_dep, a_dep);
        n_exec = 1;
    }
    Codelet(){};
    ~Codelet(){};
    void dec_Dep();
    void init_Codelet(uint32_t a_dep, uint32_t a_res, ThreadedProcedure * a_TP, codelet_status a_status){
      s_slot.init_SS(a_dep, a_dep);
      c_status = a_status;
      t_TP = a_TP;
    }
    void reset_Codelet(){
      s_slot.reset_Counter();
    }
    void set_status(codelet_status a_status){ c_status = a_status;}
    void set_id(int a_id){ cd_id = a_id;}
    void set_TP(ThreadedProcedure * a_TP){ t_TP = a_TP;}
    codelet_status get_status(){ return c_status;}
    ThreadedProcedure * get_TP(){ return t_TP;}
    uint32_t get_Dep(){ return s_slot.get_Counter();}
    SyncSlot * get_SyncSlot(){ return &s_slot;}
    ComputationalUnit * get_CU(){ return t_CU;}
    int get_id(){ return cd_id;};
    int get_tpid();
    int get_tpid_full();
    int get_nexec(){ return n_exec;};
    void exe(){
      this->stus_actv();
      this->fire();
      this->dec_nexec();
    }
    // Set status DRMT
    void stus_drmt(){ this->c_status = C_DRMT;}
    // Set status ENBL
    void stus_enbl(){ this->c_status = C_ENBL;}
    // Set status FIRE
    void stus_fire(){ this->c_status = C_FIRE;}
    // Set status ACTV
    void stus_actv(){ this->c_status = C_ACTV;}
    void set_CU(ComputationalUnit * a_CU){ this->t_CU = a_CU;}
    int dec_nexec();
    virtual void fire(void) = 0; // Codelet firing rule
    void * invoke(int TP_id);
  };
  // Codelet Vector
  typedef std::vector<Codelet*> cd_v;

  // Codelet Queue
  typedef thread_safe::deque<Codelet*> cd_q;
}
#endif /* CODELET_H */