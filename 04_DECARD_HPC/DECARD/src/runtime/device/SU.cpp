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
  Codelet * newCD;

  // SU initialize 
  DECARD_INFOMSG(1, "%s: SU: INIT", n_int->node_name);
  do{
    switch(s_mode) {
      case S_IDLE: // Idle Mode
        DECARD_INFOMSG(1, "%s: SU: IDLE ISC=%03d OSC=%03d IST=%03d OST=%03d", n_int->node_name, 
        t_ISCLQ->size(), t_OSCLQ->size(), t_ISTPQ->size(), t_OSTPQ->size());
        // Evaluate conditions for next state
        if(!t_ISTPQ->empty()){
          for (tps_it = t_ISTPQ->begin(); tps_it != t_ISTPQ->end(); ++tps_it){
            // Get TP's Codelet Queue
            t_cds = (*tps_it)->get_cdv();
            for (cds_it = t_cds->begin(); cds_it != t_cds->end(); ++cds_it){
              if ((*cds_it)->get_status() == C_ENBL){
                DECARD_INFOMSG(1, "%s: SU_Sch: CD_rdy CD_%03d", n_int->node_name, (*cds_it)->get_id());
                // Return Codelet
              }
            }
          }
        // if(!t_ISTPQ->empty()){  
        //   // Available TP Closure 
        //   // Check for enabled Codelets
        
        //   // Sort Codelets
        //   sch.sort_codelets();
          
        //   // -> Switch to Init TP
        //   this->mode_itp();
        // }else if(sch.cd_rdy()){
        //   // Codelet Ready -> Switch to Push codelet
        //   this->mode_pcd();
        }else if(t_ISTPQ->size() > this->max_istpq){
          // ISTPQ > MAX Scheduler TP -> Switch to remote
          this->mode_rmt();
        }
        if (this->get_mode() == S_IDLE){
          // Stay in IDLE
          // ToDo: Add conditional for verbose mode
          usleep(1000000);
        }
      break;

    case S_INTP: // Init TP Mode
      DECARD_INFOMSG(1, "%s: SU: INTP", n_int->node_name);
        if(!t_ISTPQ->empty()){
          DECARD_INFOMSG(1, "%s: SU: INTP TP Avail", n_int->node_name);
          // ToDo: Don't pop tp until it is done 
          newTP = t_ISTPQ->popFront();
          if (newTP->get_ncd() && sch.cd_rdy()){
            // Check if there is an avalable CU
            if (t_CU->is_avail()){
              // Assign Codelets to CU
              newCD = sch.get_rdy();
              // sch.get_rdy();
              t_CU->add_cd(newCD);
            }
          }
        }
      // Allocate Memory
      // Check Codelets
      s_mode = S_IDLE;
      break;

    case S_PSCD: // Push Codelet to CDQ
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
