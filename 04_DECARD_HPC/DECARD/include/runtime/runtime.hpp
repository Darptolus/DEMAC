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
  class Runtime
  {
    private:
      AbstractMachine DECARD_AM;
    public:
      Runtime(){};
      ~Runtime(){};
      int run();
      int end();
  };
}
#endif /* RUNTIME_H */