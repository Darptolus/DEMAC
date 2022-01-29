#include "decard.hpp"

using namespace decard;

int main(int argc, char *argv[]){

  Runtime decard_rt{};
  int w_size, n_rank, x, y, oopr;
  ThreadedProcedure * newTP;

  n_rank = decard_rt.get_worldrank();
  w_size = decard_rt.get_worldsize();
  y = 1;
  x = 0;
  // Generate TPs
  do{
    for (int i = 0; i < w_size; ++i){
      if (i != n_rank){
        if (x==y){
          // End TP
          oopr = 999;
        }else{
          oopr = (i + 1) * 1000 + (n_rank + 1) * 10 + x;
        }
        newTP = new ThreadedProcedure();
        newTP->set_opr(oopr);
        newTP->set_orig(n_rank);
        newTP->set_dest(i);
        DECARD_INFOMSG(1, "%s: Generating M_%04d", decard_rt.hostname, oopr);
        decard_rt.add_TP(newTP);
        
      }
    }
    ++x;
  }while(x <= y);
  // Start Runtime
  decard_rt.run();
  // Finalize Runtime
  decard_rt.end();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}