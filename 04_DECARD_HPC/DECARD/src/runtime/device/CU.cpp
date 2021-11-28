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

int SU::run()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  
  ThreadedProcedure * newTP;

  // SU initialize 
  DECARD_INFOMSG(1, "%s: SU: INIT", n_int->node_name);
  // DECARD_INFOMSG(1, "This is a test");
  do{
    switch(cu_mod) {
    case C_IDLE: // Idle Mode
      DECARD_INFOMSG(1, "%s: SU: IDLE", n_int->node_name);
      // 
      
      break;

    case C_EXEC: // Init TP Mode
      DECARD_INFOMSG(1, "%s: SU: INTP", n_int->node_name);
      if(){  // Available TP Closure
        // Init TP
        s_mode = S_INTP;
      }else if(){ // Codelet Ready
        // Push codelet
        s_mode = S_PCDT;
      }

      break;

    case C_IVTP:
    DECARD_INFOMSG(1, "%s: SU: PCDT", n_int->node_name);
      
      break;

    default:
      DECARD_INFOMSG(1, "%s: SU: Invalid State", n_int->node_name);
    }
  }while (n_int->get_exec());

  return 0;
}