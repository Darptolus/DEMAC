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
#include "NodeInterface.hpp"
#include "ThreadedProcedure.hpp"


namespace decard
{
  class NMGR
  {
    private:
      Node * this_node;
      AllNodes * nodes_list;
      AllNodes::iterator n_it;
      control_q * this_ICTRQ;
      control_q * this_OCTRQ;
    public:
      NMGR(AllNodes * a_nodes, Node * a_node, control_q * a_ICTRQ, control_q * a_OCTRQ):
           nodes_list(a_nodes), this_node(a_node), this_ICTRQ(a_ICTRQ), this_OCTRQ(a_OCTRQ){};
      ~NMGR(){};
      int run();
      int tst_gen();
  };
}

#endif /* NMGR_H */