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

namespace decard
{
  class NMGR
  {
    private:
      Node * t_node;
      AllNodes * nodes_list;
      AllNodes::iterator n_it;
      control_q * t_ICTRQ;
      control_q * t_OCTRQ;
      tp_q * t_INTPQ;
      tp_q * t_ONTPQ;
      tp_q * t_ISTPQ;
      tp_q * t_OSTPQ;
    public:
      NMGR(AllNodes * a_nodes, Node * a_node, 
           control_q * a_ICTRQ, control_q * a_OCTRQ,
           tp_q * a_INTPQ, tp_q * a_ONTPQ, tp_q * a_ISTPQ, tp_q * a_OSTPQ):
           nodes_list(a_nodes), t_node(a_node), t_ICTRQ(a_ICTRQ), t_OCTRQ(a_OCTRQ),
           t_INTPQ(a_INTPQ), t_ONTPQ(a_ONTPQ), t_ISTPQ(a_ISTPQ), t_OSTPQ(a_OSTPQ){};
      ~NMGR(){};
      int run();
      int tst_gen();
      int tst_gen_0();
  };
}

#endif /* NMGR_H */