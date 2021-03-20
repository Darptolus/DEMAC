/**
 * @file NCOM.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Comunicator
 * @todo Add copyright
 *
 * Includes the definition of the Node Comunicator for each node of the cluster
 */

#ifndef NCOM_H
#define NCOM_H

#include <stdio.h>
#include <stdlib.h>
#include "NodeInterface.hpp"
#include <mpi.h>
#include <vector>
#include <iterator>

namespace decard
{
  class NCOM
  {
    private:
      Node * t_node;
      AllNodes * nodes_list;
      AllNodes::iterator n_it;
      control_q * t_ICTRQ;
      control_q * t_OCTRQ;
      tp_q * t_INTPQ;
      tp_q * t_ONTPQ;
    public:
      NCOM(AllNodes * a_nodes, Node * a_node, 
           control_q * a_ICTRQ, control_q * a_OCTRQ,
           tp_q * a_INTPQ, tp_q * a_ONTPQ):
           nodes_list(a_nodes), t_node(a_node), t_ICTRQ(a_ICTRQ), t_OCTRQ(a_OCTRQ),
           t_INTPQ(a_INTPQ), t_ONTPQ(a_ONTPQ) {};
      ~NCOM(){};
      int run();
      int tst_gen_0();
  };
}

#endif /* NCOM_H */
