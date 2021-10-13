/**
 * @file edarts.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief eDARTS
 * @todo Add copyright
 *
 * Includes the definition of the accelerator for each node of the cluster
 */

#ifndef DDARTS_HPP
#define DDARTS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "NodeInterface.hpp"
#include "DECARD_tools.hpp"

namespace decard
{
  class dDARTS
  {
    private:
      Node * t_node;
      cl_q * t_ISCLQ; // Input Scheduler Control Queue
      cl_q * t_OSCLQ; // Output Scheduler Control Queue
      tp_q * t_ISTPQ; // Input Scheduler Threaded Procedure Queue
      tp_q * t_OSTPQ; // Output Scheduler Threaded Procedure Queue
    public:
      dDARTS( Node * a_node, 
              cl_q * a_ISCLQ, cl_q * a_OSCLQ,
              tp_q * a_ISTPQ, tp_q * a_OSTPQ):
              t_node(a_node), 
              t_ISCLQ(a_ISCLQ), t_OSCLQ(a_OSCLQ), 
              t_ISTPQ(a_ISTPQ), t_OSTPQ(a_OSTPQ)
              {};
      ~dDARTS(){};
      // unsigned get_rows(){ return rows;};
      // unsigned get_cols(){ return cols;};
      
      void init(){
        // printf("INITIALIZING eDARTS\n");
        // e_init(NULL);
        // e_reset_system();
        // e_get_platform_info(&platform);
        // rows = platform.rows;
        // cols = platform.cols;
        // ncores = rows * cols;
      };
      int run_SU();
      int run_CU();
      void p_res();
      void end();
  };
}
#endif /* eDARTS_HPP */