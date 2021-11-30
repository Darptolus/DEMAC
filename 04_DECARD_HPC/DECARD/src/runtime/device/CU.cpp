/**
 * @file CU.cpp
 * @author Diego Roa
 * @date 15 Oct 2021
 * @brief CU
 * @todo Add copyright
 *
 * Includes the definition of CU
 */


#include "CU.hpp"
#include "Node.hpp"

using namespace decard;

int CU::run()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  
  ThreadedProcedure * newTP;

  // SU initialize 
  DECARD_INFOMSG(1, "%s: SU: INIT", n_int->node_name);
  // DECARD_INFOMSG(1, "This is a test");
  do{
    switch(c_mode) {
    case C_IDLE: // Idle Mode
      DECARD_INFOMSG(1, "%s: SU: IDLE", n_int->node_name);
      if(!t_CDQ.empty()){  // Codelet Ready
        // Switch to Execute
        c_mode = C_EXEC;
      }
      break;

    case C_EXEC: // Init TP Mode
      DECARD_INFOMSG(1, "%s: SU: INTP", n_int->node_name);
      if(this->get_invTP()){  // Invoke TP
        // Switch to Invoke TP
        c_mode = C_IVTP;
      }
      break;

    case C_IVTP:
      DECARD_INFOMSG(1, "%s: SU: PCDT", n_int->node_name);
      c_mode = C_IDLE;
      break;

    default:
      DECARD_INFOMSG(1, "%s: SU: Invalid State", n_int->node_name);
      c_mode = C_IDLE;
    }
  }while (n_int->get_exec());

  return 0;
}