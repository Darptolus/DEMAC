/**
 * @file CU.hpp
 * @author Diego Roa
 * @date 15 Oct 2021
 * @brief CU
 * @todo Add copyright
 *
 * Includes the definition of CU
 */

#ifndef CU_H
#define CU_H

#include <stdio.h>
#include <stdlib.h>
#include "NodeInterface.hpp"
#include "Codelet.hpp"

namespace decard
{
  enum cu_mode
  {
    C_IDLE, // Computational Unit Idle
    C_EXEC, // Computational Unit Init TP
    C_IVTP, // Computational Unit Push Codelet
  };

  class CU
  {
  private:
    cu_mode c_mode;
    Node * t_node; // This node
    cd_q t_CDQ; // Codelet Queue
  public:
    CU(Node * a_node):
    t_node(a_node){};
    ~CU(){};
    void set_mode(cu_mode a_c_mode){
      this->c_mode = a_c_mode;
    };
    cu_mode get_mode(){ return c_mode;};
  };
}

#endif /* CU_H */