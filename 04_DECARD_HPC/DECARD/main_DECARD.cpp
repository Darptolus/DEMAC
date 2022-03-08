#include "decard.hpp"

using namespace decard;

Runtime decard_rt{};
class cd_01 : public Codelet
{
  public:
  cd_01(ThreadedProcedure * myTP, uint32_t dep):
  Codelet(myTP, dep){}
  // cd_01(ThreadedProcedure * myTP, codelet_status stat, uint32_t dep, uint32_t res):
  // Codelet(myTP, stat, dep, res){}

  void fire(){
    DECARD_INFOMSG(1, "%s: CD_01 ", decard_rt.hostname);
  };
};

//This codelet is the continuation of the intiated split phase
class cd_02 : public Codelet
{
public:
  cd_02(ThreadedProcedure * myTP, uint32_t dep):
  Codelet(myTP, dep){}

  void fire(){
    DECARD_INFOMSG(1, "%s: CD_02 ", decard_rt.hostname);
  };
};

class cd_03 : public Codelet
{
public:
  cd_03(ThreadedProcedure * myTP, uint32_t dep):
  Codelet(myTP, dep){}

  void fire(){
    DECARD_INFOMSG(1, "%s: CD_03 ", decard_rt.hostname);
  };
};

//This is the Hello World Threaded Procedure
class HelloWorld : public ThreadedProcedure
{
public:
  cd_01 cd_01_a;
  cd_02 cd_02_a;
  cd_03 cd_03_a;

  HelloWorld(int n_nodes):
  ThreadedProcedure(),
  cd_01_a(this, 0), // TP, dependencies
  cd_02_a(this, n_nodes), // TP, dependencies
  cd_03_a(this, 1) // TP, dependencies
  {  
    add_cd(&cd_01_a);
    add_cd(&cd_02_a);
    add_cd(&cd_03_a);
  }
};

int main(int argc, char *argv[]){

  int w_size, n_rank, m_tps, m_cd, i_rank, i_tp, i_cd, oopr;
  ThreadedProcedure * newTP;
  Codelet * newCD;
  tp_type tptype;

  n_rank = decard_rt.get_worldrank();
  w_size = decard_rt.get_worldsize();
  m_tps = 1;
  m_cd = 1;
  // Generate TPs
  for(i_tp = 0; i_tp < m_tps; ++i_tp){
    // For each node
    for (i_rank = 0; i_rank < w_size; ++i_rank){
      tptype = PRT;
      oopr = (i_rank + 1) * 1000 + (n_rank + 1) * 10 + i_tp;
      // Create TP
      newTP = new ThreadedProcedure();
      newTP->set_opr(oopr);
      newTP->set_tptype(tptype);
      newTP->set_orig_id(n_rank);
      newTP->set_dest_id(i_rank);
      // Local TP only  
      if (i_rank == n_rank){
        // Add Codelets
        // for (i_cd = 0; i_cd < m_cd; ++i_cd){
          newCD = new cd_01(newTP, 0);
          newTP->add_cd(newCD);
          newCD = new cd_02(newTP, 0);
          newTP->add_cd(newCD);
          newCD = new cd_03(newTP, 0);
          newTP->add_cd(newCD);
          // newCD = new Codelet(newTP, 0);
          // newCD->set_id(i_cd + 1);
          // Add codelet
          // newTP->add_cd(newCD);
        // }
      }
      DECARD_INFOMSG(1, "%s: Generating TP_%04d", decard_rt.hostname, oopr);
      decard_rt.add_TP(newTP);
         
        
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
  DECARD_INFOMSG(1, "%s: Generating TP_%04d", decard_rt.hostname, oopr);
  decard_rt.add_TP(newTP);

  // Start Runtime
  decard_rt.run();
  // Finalize Runtime
  decard_rt.end();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}