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

int Runtime::run()
{
  DECARD_AM.run();
  return 0;
}

int Runtime::end()
{
  DECARD_AM.end();
  return 0;
}

