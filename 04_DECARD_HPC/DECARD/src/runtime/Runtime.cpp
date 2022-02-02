/**
 * @file runtime.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Runtime
 * @todo Add copyright
 *
 * Includes the definition of the Runtime
 */

#include "Runtime.hpp"

using namespace decard;

int Runtime::run()
{
  DECARD_INFOMSG(1, "%s: Starting Runtime ", this->hostname);
  DECARD_CAM.run();
  return 0;
}

int Runtime::end()
{
  DECARD_INFOMSG(1, "%s: Ending Runtime ", this->hostname);
  DECARD_CAM.end();
  return 0;
}

