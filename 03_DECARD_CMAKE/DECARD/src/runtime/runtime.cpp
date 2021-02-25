/**
 * @file runtime.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Runtime
 * @todo Add copyright
 *
 * Includes the definition of the Runtime
 */

#include "runtime.hpp"

using namespace decard;

int Runtime::start_rt()
{
  AbstractMachine DECARD_AM;
  // Initialize Runtime
  printf("INITIALIZING RUNTIME\n");
  DECARD_AM.start_AM();
  
  return 0;
}

int Runtime::end_rt()
{
  return 0;
}