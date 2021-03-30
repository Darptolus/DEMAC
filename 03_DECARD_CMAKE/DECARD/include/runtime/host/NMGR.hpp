/**
 * @file NMGR.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node Manager
 * @todo Add copyright
 *
 * Includes the definition of the Node Manager for each node of the cluster
 */

#ifndef NMGR_H
#define NMGR_H

#include <stdio.h>
#include <stdlib.h>
#include "NodeInterface.hpp"

namespace decard
{
  enum nmgr_mode
  {
    M_DONE,
    M_IDLE,
    M_LOCAL,
    M_REMOTE,
  };

  class NMGR
  {
  private:
    Node * t_node;
    AllNodes * nodes_list;
    AllNodes::iterator n_it;
    control_q * t_ICTRQ;
    control_q * t_OCTRQ;
    tp_q * t_INTPQ;
    tp_q * t_ONTPQ;
    tp_q * t_ISTPQ;
    tp_q * t_OSTPQ;
    nmgr_mode m_mode;
    int maxltp;
  public:
    NMGR(AllNodes * a_nodes, Node * a_node, 
          control_q * a_ICTRQ, control_q * a_OCTRQ,
          tp_q * a_INTPQ, tp_q * a_ONTPQ, tp_q * a_ISTPQ, tp_q * a_OSTPQ):
          nodes_list(a_nodes), t_node(a_node), t_ICTRQ(a_ICTRQ), t_OCTRQ(a_OCTRQ),
          t_INTPQ(a_INTPQ), t_ONTPQ(a_ONTPQ), t_ISTPQ(a_ISTPQ), t_OSTPQ(a_OSTPQ){
            m_mode = M_REMOTE;
            maxltp = 0; // Max number of local TPS in OSTPQ
          };
    ~NMGR(){};
    int run();
    void set_mode(nmgr_mode a_m_mode){
      this->m_mode = a_m_mode;
    };
    void set_mltp(int a_maxltp){
      this->maxltp = a_maxltp;
    };
    nmgr_mode get_mode(){ return m_mode;};
    int get_mltp(){ return maxltp;};
    void mode_dne(){
      this->m_mode = M_DONE;
    };
    void mode_idl(){
      this->m_mode = M_IDLE;
    };
    void mode_lcl(){
      this->m_mode = M_LOCAL;
    };
    void mode_rmt(){
      this->m_mode = M_REMOTE;
    };
    // int tst_gen();
    int tst_gen_0();
    int tst_gen_1(int x);
};
}

#endif /* NMGR_H */