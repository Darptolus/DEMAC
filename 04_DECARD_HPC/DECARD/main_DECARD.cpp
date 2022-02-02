#include "decard.hpp"

using namespace decard;

int main(int argc, char *argv[]){

  Runtime decard_rt{};
  int w_size, n_rank, x, m_tps, oopr;
  ThreadedProcedure * newTP;
  tp_type tptype;

  n_rank = decard_rt.get_worldrank();
  w_size = decard_rt.get_worldsize();
  m_tps = 1;
  x = 0;
  // Generate TPs
  for(x = 0; x < m_tps; ++x){
    // For each node
    for (int i = 0; i < w_size; ++i){
      if (i != n_rank){
        tptype = PRT;
        oopr = (i + 1) * 1000 + (n_rank + 1) * 10 + x;
        // Create TP
        newTP = new ThreadedProcedure();
        newTP->set_opr(oopr);
        newTP->set_tptype(tptype);
        newTP->set_orig_id(n_rank);
        newTP->set_dest_id(i);
        DECARD_INFOMSG(1, "%s: Generating M_%04d", decard_rt.hostname, oopr);
        decard_rt.add_TP(newTP);
      }
    }
  }
  tptype = END;
  oopr = 999;
  // Create TP
  newTP = new ThreadedProcedure();
  newTP->set_opr(oopr);
  newTP->set_tptype(tptype);
  newTP->set_orig_id(n_rank);
  newTP->set_dest_id(n_rank);
  DECARD_INFOMSG(1, "%s: Generating M_%04d", decard_rt.hostname, oopr);
  decard_rt.add_TP(newTP);

  // Start Runtime
  decard_rt.run();
  // Finalize Runtime
  decard_rt.end();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}