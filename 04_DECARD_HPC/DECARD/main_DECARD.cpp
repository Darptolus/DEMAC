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
    DECARD_INFOMSG(1, "%s: CD_02 Invoking TPs... ", decard_rt.hostname);
    // Invoke SayHello per node
    // this->invoke(1);
  };
};

class cd_03 : public Codelet // set number of dependencies = n_nodes
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
  HelloWorld(){};
  HelloWorld(int n_nodes):
  ThreadedProcedure()
  {
    add_cd(new cd_01(this, 0));
    add_cd(new cd_02(this, 0));
    add_cd(new cd_03(this, 0));
  };
  virtual ~HelloWorld() {
    // DECARD_INFOMSG(1, "%s: TP Destructor ", decard_rt.hostname);
  };
  virtual HelloWorld * clone(){ return new HelloWorld();};
};

class SayHello : public ThreadedProcedure
{
public:
  SayHello():
  ThreadedProcedure()
  {  
    // Add Codelets
    add_cd(new cd_04(this, 0));
  }
  virtual ~SayHello() {};
  virtual SayHello * clone(){ return new SayHello();};
};

int main(int argc, char *argv[]){

  int w_size, n_rank, m_tps, m_cd, i_rank, i_tp, i_cd, tp_id;
  ThreadedProcedure * newTP;
  Codelet * newCD;
  tp_type tptype;

  n_rank = decard_rt.get_worldrank();
  w_size = decard_rt.get_worldsize();
  // m_tps = 1;
  // m_cd = 1;
  // Generate TPs
  newTP = new HelloWorld(w_size);
  newTP->set_tptype(tptype);
  newTP->set_orig_id(n_rank);
  newTP->set_dest_id(n_rank);
  decard_rt.add_TP(newTP);

  DECARD_INFOMSG(1, "%s: Generating TP_%05d", decard_rt.hostname, newTP->get_id_full());  
  
  newTP = new SayHello();
  newTP->set_tptype(tptype);
  newTP->set_orig_id(n_rank);
  newTP->set_dest_id(-1);
  decard_rt.add_TP(newTP);

  DECARD_INFOMSG(1, "%s: Generating TP_%05d", decard_rt.hostname, newTP->get_id_full()); 

  // for(i_tp = 0; i_tp < m_tps; ++i_tp){
  //   // For each node
  //   for (i_rank = 0; i_rank < w_size; ++i_rank){
  //     tptype = PRT;
  //     // tp_id = (i_rank + 1) * 1000 + (n_rank + 1) * 10 + i_tp;
  //     // Create TPs
  //     if (i_rank == n_rank){
  //       // Local TPs
  //       // newTP = new HelloWorld(w_size);
  //       // newTP = new ThreadedProcedure();
  //     }else{
  //       // Remote TPs
  //       newTP = new SayHello();
  //       // newTP = new ThreadedProcedure();
  //     }
  //     // newTP->set_id(tp_id);
  //     newTP->set_tptype(tptype);
  //     newTP->set_orig_id(n_rank);
  //     newTP->set_dest_id(i_rank);
  //     decard_rt.add_TP(newTP);
  //     DECARD_INFOMSG(1, "%s: Generating TP_%05d", decard_rt.hostname, newTP->get_id_full());  
  //     }
  //   }

  tptype = END;
  // tp_id = 999;
  // Create TP
  newTP = new ThreadedProcedure();
  // newTP->set_id(tp_id);
  newTP->set_tptype(tptype);
  newTP->set_orig_id(n_rank);
  newTP->set_dest_id(n_rank);
  decard_rt.add_TP(newTP);
  DECARD_INFOMSG(1, "%s: Generating TP_%05d -> END TP", decard_rt.hostname, newTP->get_id_full());

  // Define Dependencies
  // Start Runtime
  decard_rt.run();
  // Finalize Runtime
  decard_rt.end();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}