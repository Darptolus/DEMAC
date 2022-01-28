/**
 * @file CodeletGraph.hpp
 * @author Diego Roa
 * @date 3 Dec 2021
 * @brief Codelet Graph
 * @todo Add copyright
 *
 * Includes the definition of the Codelet Graph for the runtime
 */

#ifndef CODELETGRAPH_H
#define CODELETGRAPH_H

#include <stdlib.h>
#include <vector>
#include <iterator>
#include "thread_safe_deque.h"
#include "NodeInterface.hpp"
#include "DECARD_tools.hpp"

namespace decard
{
  class CodeletGraph
  {
  private:
    int num_tp;
    int num_cd;
    tp_q all_tps;
    tp_q::iterator tps_it;
    // AllTPs node_tps;
    // AllTPs::iterator ntps_it;
  public:
    CodeletGraph(){
      num_tp = 0;
    };
    ~CodeletGraph() {
      for (auto& element : all_tps) {
        delete element;
      }
    };
    void add_tp(ThreadedProcedure * a_TP){
      all_tps.push_back(a_TP);
      ++num_tp;
    };
    tp_q * get_atps() {return &all_tps;};
    int get_tps_size() {return num_tp;};
    // void get_tps (Node * a_node, tp_q * a_tpq);
    // ThreadedProcedure * get_tps (Node * a_node);
    
  }; 
}
#endif /* CODELETGRAPH_H */

