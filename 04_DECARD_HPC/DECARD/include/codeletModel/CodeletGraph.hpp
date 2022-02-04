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
// #include "thread_safe_deque.h"
#include "ThreadedProcedure.hpp"
#include "DECARD_tools.hpp"
// #include "NodeInterface.hpp"

namespace decard
{
  class CodeletGraph
  {
  private:
    int num_tp; // Number of TPs
    int num_cd; // Number of Codelets
    tp_q all_tps; // Contains all TPS
    tp_q::iterator tps_it;
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
  }; 
}
#endif /* CODELETGRAPH_H */

