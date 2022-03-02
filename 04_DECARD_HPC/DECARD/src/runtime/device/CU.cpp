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

int ComputationalUnit::run()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  ThreadedProcedure * newTP;
  Codelet * newCD;

  // SU initialize 
  DECARD_INFOMSG(1, "%s: CU: INIT", n_int->node_name);
  // DECARD_INFOMSG(1, "This is a test");
  do{
    switch(this->get_mode()) {
    case U_IDLE: // Idle Mode
      DECARD_INFOMSG(1, "%s: CU: IDLE", n_int->node_name);
      if(!CDQ.empty()){  // Codelet Ready
        // Switch to Execute
        DECARD_INFOMSG(1, "%s: CU: IDLE NOT EMPTY", n_int->node_name);
        this->mode_exe();
      }
      if (this->get_mode() == U_IDLE){
        // Stay in IDLE
        usleep(1000000);
      }
      break;

    case U_EXEC: // Init TP Mode
      DECARD_INFOMSG(1, "%s: CU: EXEC", n_int->node_name);
      if(!CDQ.empty()){
        newCD = CDQ.popFront();
        DECARD_INFOMSG(1, "%s: CU: EXEC CD_%03d", n_int->node_name, newCD->get_id());
        if(this->get_invTP()){  // Invoke TP
          // Switch to Invoke TP
          this->mode_ivt();
        }
        // Execute Codelet -> Decrease execution counter
        newCD->exe();
        if (!newCD->get_nexec()){
          // Execution Complete -> Codelet Dormant
          DECARD_INFOMSG(1, "%s: CU: EXEC CD_%03d Over", n_int->node_name, newCD->get_id());
          newCD->stus_drmt();
        }
      }
      this->mode_idl();
      break;

    case U_IVTP:
      DECARD_INFOMSG(1, "%s: CU: IVTP", n_int->node_name);
      this->mode_idl();
      break;

    default:
      DECARD_INFOMSG(1, "%s: SU: Invalid State", n_int->node_name);
      this->mode_idl();
    }
  }while (n_int->get_exec());

  return 0;
}