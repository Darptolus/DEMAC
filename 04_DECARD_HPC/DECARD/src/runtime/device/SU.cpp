/**
 * @file SU.cpp
 * @author Diego Roa
 * @date 15 Oct 2021
 * @brief SU
 * @todo Add copyright
 *
 * Includes the definition of SU
 */

#include "SU.hpp"
#include "Node.hpp"

using namespace decard;

int SU::run(){
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  ThreadedProcedure * newTP;

  // SU initialize 
  DECARD_INFOMSG(1, "%s: SU: INIT", n_int->node_name);
  do{
    switch(s_mode) {
      case S_IDLE: // Idle Mode
        DECARD_INFOMSG(1, "%s: SU: IDLE", n_int->node_name);
        // Check for enabled Codelets
        // Sort Codelets
        sch.sort_codelets();
        // Evaluate conditions for next state
        if(!t_ISTPQ->empty()){  // Available TP Closure
          // Switch to Init TP
          this->mode_itp();
        }else if(sch.get_CDrdy()){ // Codelet Ready
          // Switch to Push codelet
          this->mode_pcd();
        }else if(t_ISTPQ->size() > this->max_istpq){ // ISTPQ > MAX Scheduler TP 
          // Switch to remote
          this->mode_rmt();
        }
        if (this->get_mode() == S_IDLE){
          // Stay in IDLE
          usleep(1000000);
        }
      break;

    case S_INTP: // Init TP Mode
      DECARD_INFOMSG(1, "%s: SU: INTP", n_int->node_name);
        if(!t_ISTPQ->empty()){
          newTP = t_ISTPQ->popFront();
        }
      // Allocate Memory
      // Check Codelets
      s_mode = S_IDLE;
      break;

    case S_PCDT: // Push Codelet to CDQ
      DECARD_INFOMSG(1, "%s: SU: PCDT", n_int->node_name);
      // Check for available CU
      s_mode = S_IDLE;
      break;

    case S_REMT:
      DECARD_INFOMSG(1, "%s: SU: REMT", n_int->node_name);
      // Push TP to OSTPQ
      s_mode = S_IDLE;
      break;

    default:
      DECARD_INFOMSG(1, "%s: SU: Invalid State", n_int->node_name);
    }
  }while (n_int->get_exec());

  return 0;
}
