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

class cd_04 : public Codelet
{
public:
  cd_04(ThreadedProcedure * myTP, uint32_t dep):
  Codelet(myTP, dep){}

  void fire(){
    DECARD_INFOMSG(1, "%s: CD_04 ", decard_rt.hostname);
  };
};


//This is the Hello World Threaded Procedure
class HelloWorld : public ThreadedProcedure
{
public:
  HelloWorld(int n_nodes):
  ThreadedProcedure()
  {
    add_cd(new cd_01(this, 0));
    // add_cd(&cd_02_a);
    // add_cd(&cd_03_a);
  };
  virtual ~HelloWorld() {
    // DECARD_INFOMSG(1, "%s: TP Destructor ", decard_rt.hostname);
    // this->clear_all();
  };
};

class SayHello : public ThreadedProcedure
{
public:
  cd_04 cd_04_a;

  SayHello():
  ThreadedProcedure(),
  cd_04_a(this, 1) // TP, dependencies
  {  
    add_cd(&cd_04_a);
  }
};

int main(int argc, char *argv[]){

  int w_size, n_rank, m_tps, m_cd, i_rank, i_tp, i_cd, tp_id;
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
      tp_id = (i_rank + 1) * 1000 + (n_rank + 1) * 10 + i_tp;
      // Create TP
      
      // Local TP only  
      if (i_rank == n_rank){
        // Add Codelets
        // for (i_cd = 0; i_cd < m_cd; ++i_cd){
          // newCD->set_id(i_cd + 1);
          // Add codelet
          // newTP->add_cd(newCD);
        // }
        // newCD = new cd_01(newTP, 0);
        // newTP->add_cd(newCD);
        // newCD = new cd_02(newTP, 0);
        // newTP->add_cd(newCD);
        // newCD = new cd_03(newTP, 0);
        // newTP->add_cd(newCD);
        // newCD = new Codelet(newTP, 0);
        newTP = new HelloWorld(w_size);
      }else{
        newTP = new ThreadedProcedure();
      }
      newTP->set_opr(tp_id);
      newTP->set_tptype(tptype);
      newTP->set_orig_id(n_rank);
      newTP->set_dest_id(i_rank);
      DECARD_INFOMSG(1, "%s: Generating TP_%04d", decard_rt.hostname, tp_id);
      decard_rt.add_TP(newTP);
         
        
      }
    }

  tptype = END;
  tp_id = 999;
  // Create TP
  newTP = new ThreadedProcedure();
  newTP->set_opr(tp_id);
  newTP->set_tptype(tptype);
  newTP->set_orig_id(n_rank);
  newTP->set_dest_id(n_rank);
  DECARD_INFOMSG(1, "%s: Generating TP_%04d", decard_rt.hostname, tp_id);
  decard_rt.add_TP(newTP);

  // Start Runtime
  decard_rt.run();
  // Finalize Runtime
  decard_rt.end();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}