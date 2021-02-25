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
#include "NodeInit.hpp"
#include <vector>

namespace decard
{
  class NCOM
  {
    private:
      AllNodes * nodes_list;
      Node * this_node;
    public:
      NCOM(AllNodes * a_nodes, Node * a_node):nodes_list(a_nodes), this_node(a_node){};
      ~NCOM(){};
      int start_NCOM();

  };
}

#endif /* NCOM_H */
