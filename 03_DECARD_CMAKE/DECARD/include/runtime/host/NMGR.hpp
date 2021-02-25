/**
 * @file NMGR.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Manager
 * @todo Add copyright
 *
 * Includes the definition of the Node Manager for each node of the cluster
 */

#ifndef NMGR_H
#define NMGR_H

#include <stdio.h>
#include <stdlib.h>

namespace decard
{
  class NMGR
  {
    private:
      
    public:
      NMGR(){};
      ~NMGR(){};
      int start_NMGR();
  };
}

#endif /* NMGR_H */