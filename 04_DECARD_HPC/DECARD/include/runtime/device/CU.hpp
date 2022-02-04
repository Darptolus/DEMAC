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
    U_IDLE, // Computational Unit Idle
    U_EXEC, // Computational Unit Init TP
    U_IVTP, // Computational Unit Push Codelet
  };

  class CU
  {
  private:
    cu_mode u_mode;
    Node * t_node; // This node
    cd_q t_CDQ; // Codelet Queue
    bool inv_TP; // Invoke TP
  public:
    CU(
      Node * a_node):
      t_node(a_node)
      {
        u_mode = U_IDLE;
      };
    ~CU(){};
    int run();
    bool get_invTP()
    {
      inv_TP = 0;
      return inv_TP;
    };
    void set_mode(cu_mode a_u_mode){ this->u_mode = a_u_mode;};
    cu_mode get_mode(){ return u_mode;};
    void mode_idl(){ this->u_mode = U_IDLE;};
    void mode_exe(){ this->u_mode = U_EXEC;};
    void mode_ivt(){ this->u_mode = U_IVTP;};
  };
}

#endif /* CU_H */