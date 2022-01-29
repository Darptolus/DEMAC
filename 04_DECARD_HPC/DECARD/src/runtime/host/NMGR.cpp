/**
 * @file NMGR.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Manager
 * @todo Add copyright
 *
 * Includes the definition of the Node Manager for each node of the cluster
 */

#include "NMGR.hpp"
#include "Node.hpp"

using namespace decard;

int NMGR::get_tps(){
  // Assigns TPs to corresponding TPQ
 Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  for (tps_it = all_tps->begin(); tps_it != all_tps->end(); ++tps_it){
    if((*tps_it)->get_dest_id() == n_int->get_id()){
      DECARD_INFOMSG(1, "%s: TP assigned to INTPQ", n_int->node_name);
      //t_INTPQ->push_back(*tps_it);
    }else if((*tps_it)->get_dest_id() != n_int->get_id()){
      DECARD_INFOMSG(1, "%s: TP assigned to ONTPQ", n_int->node_name);
      t_ONTPQ->push_back(*tps_it);
    }else{
      DECARD_INFOMSG(1, "%s: TP not assigned", n_int->node_name);
    }
  }
  return 0;
}

int NMGR::run()
{
  int y = 1; //tobedeleted, for testing
  // Initialize NMGR 
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  Node_Extern * n_ext;
  ThreadedProcedure * newTP;
  int x, oopr;
  int done;
  DECARD_INFOMSG(1, "%s: NMGR: INIT", n_int->node_name);
  do{
    switch(this->get_mode()) {
      case M_DONE: // Done Mode
      DECARD_INFOMSG(1, "%s: NMGR: DONE", n_int->node_name);
      // Check if all nodes are DONE
      done = 1;
      for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
        if ((*n_it)->get_id() != n_int->node_id){
          n_ext = dynamic_cast <Node_Extern *> (*n_it);
          // Check node status
          if (n_ext->get_mode() != N_DONE){
            done = 0;
          }
        }
      }
      if(done && n_int->get_cidle() && t_OSTPQ->empty() && t_ISTPQ->empty()){
        // Scheduling queues are empty -> Set Node Mode DONE 
        DECARD_INFOMSG(1, "%s: NMGR: ALL DONE", n_int->node_name);
        n_int->mode_dne();
        n_int->clr_exec();
        usleep(1000000);
      }else {
        // Change to IDLE
        this->mode_idl();
      }
      break; // End Done Mode

      case M_IDLE: // Idle Mode
      DECARD_INFOMSG(1, "%s: NMGR: IDLE", n_int->node_name);
      if (t_OSTPQ->size() > get_mx_ostpq()){
        // OSTPQ > MAX Local TP -> Change to REMOTE
        this->mode_rmt();
      } else if (!t_INTPQ->empty()){
        // INTPQ > 0 -> Change to LOCAL
        this->mode_lcl();
      } else if (this->get_mode() == M_IDLE){
        // Stay in IDLE
        usleep(1000000);
      } else{
        // INVALID
        DECARD_INFOMSG(1, "%s: NMGR: Invalid State", n_int->node_name);
        usleep(1000000);
      }
      break; // End Idle Mode

      case M_LOCL: // Local Mode
      DECARD_INFOMSG(1, "%s: NMGR: LCAL", n_int->node_name);
      // Check for available SU

      // Simulate Consuming -> Assigning to SU
      if (!t_INTPQ->empty()){
        newTP = t_INTPQ->popFront();
        oopr = *(newTP->get_opr());
        DECARD_INFOMSG(1, "%s: NMGR: LCAL RO_%03d M_%04d", n_int->node_name, newTP->get_orig_id(), oopr);
      }

      if(oopr == 999){
        // "DONE" Message
        // n_it = nodes_list->at(newTP->get_orig());
        for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
          if ((*n_it)->get_id() == newTP->get_orig_id()){
            n_ext = dynamic_cast <Node_Extern *> (*n_it);
            n_ext->mode_dne();
            this->mode_dne();
          }
        }
      }else{
        this->mode_idl();
      }
      break; // End Local Mode

    case M_REMT: // Remote Mode
      DECARD_INFOMSG(1, "%s: NMGR: REMT", n_int->node_name);
      // Simulate generation -> max_LTP reached
      // x=0;
      // do{
      //   for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
      //     // printf("%s: NMGR x= %d, y= %d\n", n_int->node_name,x,y);
      //     if ((*n_it)->get_id() != n_int->node_id){
      //       n_ext = dynamic_cast <Node_Extern *> (*n_it);
      //       if (x==y){
      //         oopr = 999;
      //       }else{
      //         oopr = (n_ext->get_id() + 1) * 1000 + (n_int->node_id + 1) * 10 + x;
      //       }      
      //       // Generate TP
      //       newTP = new ThreadedProcedure();
      //       newTP->set_orig(n_int->node_id);
      //       newTP->set_dest(n_ext->get_id());
      //       newTP->set_opr(oopr);
      //       DECARD_INFOMSG(1, "%s: NMGR: REMT RD_%03d M_%04d", n_int->node_name, n_ext->get_id(), oopr);
      //       t_ONTPQ->push_back(newTP);
      //     }
      //   }
      //   ++x;
      // }while(x <= y);
      // Change to IDLE
      this->mode_idl();
      break; // End Remote Mode

    default:
      DECARD_INFOMSG(1, "%s: NMGR: Invalid State", n_int->node_name);

    } // End switch

  }while(n_int->get_exec());
  return 0;
}

int NMGR::tst_gen_0()
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  ThreadedProcedure * newTP;
  int x, y;
  // Producing
  DECARD_INFOMSG(1, "%s: NMGR: Generating TPs", n_int->node_name);
  for (x = 0; x < 3; ++x){
    newTP = new ThreadedProcedure();
    newTP->set_orig(n_int->node_id);
    // newTP->set_dest((*n_it)->get_id());
    newTP->set_opr(x);
    DECARD_INFOMSG(1, "%s: NMGR: Pushing [%d]", n_int->node_name, x);
    t_ONTPQ->push_back(newTP);
    // t_OCTRQ->push_back(x);
  }
  usleep(1000000);
  // Consuming
  DECARD_INFOMSG(1, "%s: NMGR: Consuming TPs", n_int->node_name);
  x = 0;
  while (x < 3) {
    if(!t_INTPQ->empty()){
      newTP = t_INTPQ->popFront();
      y = *(newTP->get_opr());
      DECARD_INFOMSG(1, "%s: NMGR: Popping [%d]", n_int->node_name, y);
      ++x;
    }
  }
  return 0;
}

// TP Generation test
int NMGR::tst_gen_1(int y)
{
  Node_Intern * n_int = dynamic_cast <Node_Intern *> (t_node);
  Node_Extern * n_ext;
  ThreadedProcedure * newTP;
  int x, oopr;
  int done;
  // Producing
  DECARD_INFOMSG(1, "%s: NMGR: Test_Gen TPs", n_int->node_name);

  do{
    switch(this->get_mode()) {
    case M_DONE: // Done Mode
      DECARD_INFOMSG(1, "%s: NMGR: DONE", n_int->node_name);
      // Check if all nodes are DONE
      done = 1;
      for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
        if ((*n_it)->get_id() != n_int->node_id){
          n_ext = dynamic_cast <Node_Extern *> (*n_it);
          // Check node status
          if (n_ext->get_mode() != N_DONE){
            done = 0;
          }
        }
      }
      if(done && t_OSTPQ->empty() && t_ISTPQ->empty()){
        // Scheduling queues are empty -> Set Node Mode DONE 
        n_int->mode_dne();
        usleep(1000000);
      }else {
        // Change to IDLE
        this->mode_idl();
      }
      break;

    case M_IDLE: // Idle Mode
      DECARD_INFOMSG(1, "%s: NMGR: IDLE", n_int->node_name);
      if (t_OSTPQ->size() > get_mx_ostpq()){
        // OSTPQ > MAX Local TP -> Change to REMOTE
        this->mode_rmt();
      } else if (!t_INTPQ->empty()){
        // INTPQ > 0 -> Change to LOCAL
        this->mode_lcl();
      } else if (this->get_mode() == M_IDLE){
        // Stay in IDLE
        usleep(1000000);
      } else{
        // INVALID
        DECARD_INFOMSG(1, "%s: NMGR: Invalid State", n_int->node_name);
        usleep(1000000);
      }
      break;

    case M_LOCL: // Local Mode
      DECARD_INFOMSG(1, "%s: NMGR: LCAL", n_int->node_name);
      // Check for available SU

      // Simulate Consuming -> Assigning to SU
      if (!t_INTPQ->empty()){
        newTP = t_INTPQ->popFront();
        oopr = *(newTP->get_opr());
        DECARD_INFOMSG(1, "%s: NMGR: LCAL RO_%03d M_%04d", n_int->node_name, newTP->get_orig_id(), oopr);
      }

      if(oopr == 999){
        // "DONE" Message
        // n_it = nodes_list->at(newTP->get_orig());
        for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
          if ((*n_it)->get_id() == newTP->get_orig_id()){
            n_ext = dynamic_cast <Node_Extern *> (*n_it);
            n_ext->mode_dne();
            this->mode_dne();
          }
        }
      }else{
        this->mode_idl();
      }
      
      break;

    case M_REMT: // Remote Mode
      DECARD_INFOMSG(1, "%s: NMGR: REMT", n_int->node_name);
      // Simulate generation -> max_LTP reached
      x=0;
      do{
        for (n_it = nodes_list->begin(); n_it != nodes_list->end(); ++n_it){
          // printf("%s: NMGR x= %d, y= %d\n", n_int->node_name,x,y);
          if ((*n_it)->get_id() != n_int->node_id){
            n_ext = dynamic_cast <Node_Extern *> (*n_it);
            if (x==y){
              oopr = 999;
            }else{
              oopr = (n_ext->get_id() + 1) * 1000 + (n_int->node_id + 1) * 10 + x;
            }      
            // Generate TP
            newTP = new ThreadedProcedure();
            newTP->set_orig(n_int->node_id);
            newTP->set_dest(n_ext->get_id());
            newTP->set_opr(oopr);
            DECARD_INFOMSG(1, "%s: NMGR: REMT RD_%03d M_%04d", n_int->node_name, n_ext->get_id(), oopr);
            t_ONTPQ->push_back(newTP);
          }
        }
        ++x;
      }while(x <= y);
      // Change to IDLE
      this->mode_idl();
      break;

    default:
      DECARD_INFOMSG(1, "%s: NMGR: Invalid State", n_int->node_name);
    }
  }while(n_int->get_exec());

  return 0;
}
