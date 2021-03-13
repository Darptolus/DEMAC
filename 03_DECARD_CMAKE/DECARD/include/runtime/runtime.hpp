/**
 * @file runtime.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Runtime
 * @todo Add copyright
 *
 * Includes the definition of the Runtime
 */

#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "AbstractMachine.hpp"

namespace decard
{
  // enum runtime_mode
  // {
  //   R_DONE,
  //   R_EXEC,
  //   R_IDLE,
  //   R_UNDEF 
  // };
  
  class Runtime
  {
    private:
      // runtime_mode rt_mode;
      AbstractMachine DECARD_AM;
    public:
      Runtime(){};
      // Runtime(runtime_mode a_rt_mode):rt_mode(a_rt_mode){};
      ~Runtime(){};
      int run();
      // int run(runtime_mode a_rt_mode);
      // void set_mode(runtime_mode a_rt_mode){
      //   this->rt_mode = a_rt_mode;
      // };
      int end_rt();
  };
}
#endif /* RUNTIME_H */