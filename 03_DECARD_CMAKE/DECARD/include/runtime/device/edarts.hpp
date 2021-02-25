/**
 * @file edarts.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief eDARTS
 * @todo Add copyright
 *
 * Includes the definition of the accelerator for each node of the cluster
 */

#ifndef EDARTS_HPP
#define EDARTS_HPP

#include <stdio.h>
#include <stdlib.h>

namespace decard
{
  class eDARTS
  {
    private:
    public:
      eDARTS(){};
      ~eDARTS(){};
      int start_eDARTS();
  };
}
#endif /* eDARTS_HPP */