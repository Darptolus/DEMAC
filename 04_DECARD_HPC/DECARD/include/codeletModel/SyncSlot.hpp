/**
 * @file SyncSlot.hpp
 * @author Diego Roa
 * @date 14 Oct 2021
 * @brief SyncSlot
 * @todo Add copyright
 *
 * Includes the definition of SyncSlots
 */

#include <stdint.h>
#include "Atomics.h"

#ifndef SYNCSLOT_H
#define SYNCSLOT_H

#include <stdlib.h>
#include <stdint.h>

namespace decard
{
  class SyncSlot
  {
  private:
    unsigned int SS_count;
    unsigned int SS_reset;
  public:
    SyncSlot(uint32_t a_dep, uint32_t a_res):
      SS_count(a_dep),
      SS_reset(a_res)
      {};
    ~SyncSlot(){};
    void initSS(uint32_t a_dep, uint32_t a_res){
      SS_count = a_dep;
      SS_reset = a_res;
    };
    bool decCount(){
      // Returns true when dependencies reach 0
      return (0==Atomics::fetchSub(counter_, 1U));
    };
    void resetCounter(){
      SS_count = SS_reset;
    };
    uint32_t getCounter() const{
        return SS_count;
    };
    //returns the if the counter has reached zero
    bool ready(void) const{
        return (counter_ == 0);
    } 
    
  };
}

#endif /* SYNCSLOT_H */