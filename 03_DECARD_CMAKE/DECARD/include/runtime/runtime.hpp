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
#include "AbstractMachine.hpp"

namespace decard
{
  class Runtime
  {
    private:
    public:
      Runtime(){};
      ~Runtime(){};
      int start_rt();
      int end_rt();
  };

}
#endif /* RUNTIME_H */