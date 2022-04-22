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

int SchedulingUnit::run(){
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  ThreadedProcedure * newTP;
  Codelet * newCD;

  // SU initialize 
  DECARD_INFOMSG(1, "%s: SU: INIT", n_int->node_name);
  do{
    switch(s_mode) {
      case S_DONE:
        DECARD_INFOMSG(1, "%s: SU: DONE", n_int->node_name);
        if(!ISTPQ.empty()){
          s_mode = S_IDLE;
        }else if (this->get_mode() == S_DONE){
          // Stay in DONE
          // ToDo: Add conditional for verbose mode
          usleep(1000000);
        }
      break; // End Done Scheduler

      case S_IDLE: // Idle Mode
        DECARD_INFOMSG(1, "%s: SU: IDLE ISC=%03d OSC=%03d IST=%03d OST=%03d", n_int->node_name, 
        ISCLQ.size(), OSCLQ.size(), ISTPQ.size(), OSTPQ.size());
        // ToDo: Monitor CU States
        // Evaluate conditions for next state
        if(!ISTPQ.empty()){
          // Available TP Closure
          for (tps_it = ISTPQ.begin(); tps_it != ISTPQ.end(); ){
            // Check for CDs
            DECARD_INFOMSG(1, "%s: SU: IDLE NCD=%03d", n_int->node_name, (*tps_it)->get_cdnexec());
            if (!(*tps_it)->get_cdnexec()){
              // TP Done -> Pop TP
              DECARD_INFOMSG(1, "%s: SU: IDLE TPDNE RO_%03d TP_%04d", n_int->node_name, (*tps_it)->get_orig_id(), (*tps_it)->get_id_full());
              tps_it = ISTPQ.erase(tps_it);
              break; // Exit outer loop
            }else{
              // Get TP's Codelet Queue
              t_cds = (*tps_it)->get_cdv();
              for (cds_it = t_cds->begin(); cds_it != t_cds->end(); ++cds_it){
                // Check Codelet state
                if ((*cds_it)->get_status() == C_ACTV){
                  // Codelet Active
                  DECARD_INFOMSG(1, "%s: SU: TP_%04d CD_Active CD_%03d", n_int->node_name, (*tps_it)->get_id_full(), (*cds_it)->get_id());
                }else if (!(*cds_it)->getDep() && (*cds_it)->get_nexec() && t_CU->is_avail()){
                  // Codelet Ready -> Switch to Push Codelet
                  (*cds_it)->stus_enbl();
                  this->mode_pcd();
                  DECARD_INFOMSG(1, "%s: SU: TP_%04d CD_Rdy CD_%03d", n_int->node_name, (*tps_it)->get_id_full(), (*cds_it)->get_id());break; // Exit inner loop
                  break; // Exit inner loop
                }
              } // End Codelets for loop
              if(this->get_mode() != S_IDLE){
                break; // Exit outer loop
              }
              ++tps_it; // Next TP
            }
          }// End TPs for loop
        }else if(ISTPQ.size() > this->max_istpq){
          // ISTPQ > MAX Scheduler TP -> Switch to remote
          this->mode_rmt();
        }else if(ISTPQ.empty() && t_CU->get_mode() == U_IDLE){
          // IF ISTPQ and CU(s) Done -> Set mode Done
          this->mode_dne();
        }
        // if(!ISTPQ.empty()){  
        //   // Available TP Closure 
        //   // Check for enabled Codelets
        
        //   // Sort Codelets
        //   sch.sort_codelets();
          
        //   // -> Switch to Init TP
        //   this->mode_itp();
        // }else if(sch.cd_rdy()){
        //   // Codelet Ready -> Switch to Push codelet
        //   this->mode_pcd();
        if (this->get_mode() == S_IDLE){
          // Stay in IDLE
          // ToDo: Add conditional for verbose mode
          usleep(1000000);
        }
      break; // End Idle Scheduler

    // case S_INTP: // Init TP Mode
    //   DECARD_INFOMSG(1, "%s: SU: INTP", n_int->node_name);
      // if(!ISTPQ.empty()){
      //   if (newTP->get_ncd() && sch.cd_rdy()){
      //       newCD = sch.get_rdy();
      //       // sch.get_rdy();
      //       t_CU->add_cd(newCD);
      //     }
      //   }
      // Allocate Memory
      // Check Codelets
      // s_mode = S_IDLE;
      // break;

    case S_PSCD: // Push Codelet to CDQ
      DECARD_INFOMSG(1, "%s: SU: PSCD", n_int->node_name);
      // Check for available CU
      if (t_CU->is_avail()){
        // Assign Codelets to CU
        DECARD_INFOMSG(1, "%s: SU: Assign to CU CD_%03d", n_int->node_name, (*cds_it)->get_id());
        t_CU->add_cd(*cds_it);
      }
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
