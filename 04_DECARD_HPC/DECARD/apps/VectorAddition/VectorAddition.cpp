// #include "decard.hpp"

// using namespace decard;

// Runtime decard_rt{};
// class cd_va : public Codelet
// {
//   public:
//   cd_01(ThreadedProcedure * myTP, uint32_t dep, ):
//   Codelet(myTP, dep){}
//   // cd_01(ThreadedProcedure * myTP, codelet_status stat, uint32_t dep, uint32_t res):
//   // Codelet(myTP, stat, dep, res){}

//   void fire(){
//     DECARD_INFOMSG(1, "%s: Vector Addition ", decard_rt.hostname);

//   };
// };

// //This is the Vector Addition Threaded Procedure
// class VectorAddition : public ThreadedProcedure
// {
// public:
//   VectorAddition(int n_nodes):
//   ThreadedProcedure()
//   {
//     add_cd(new cd_va(this, 0));
//   };
//   virtual ~VectorAddition() {
//     // this->clear_all();
//   };
// };

int main(int argc, char *argv[]){

//   int w_size, n_rank, m_tps, m_cd, i_rank, i_tp, i_cd, tp_id;
//   ThreadedProcedure * newTP;
//   Codelet * newCD;
//   tp_type tptype;

//   int* ptr = (int * )malloc((r * c) * sizeof(int));

//   i_tp = 1;
//   n_rank = decard_rt.get_worldrank();
//   w_size = decard_rt.get_worldsize();
//   // m_tps = 1;
//   // m_cd = 1;
//   // Generate TPs

//   tp_id = (n_rank + 1) * 1000 + (n_rank + 1) * 10 + i_tp;
//   tptype = PRT;

//   newTP = new VectorAddition();
//   newTP->set_opr(tp_id);
//   newTP->set_tptype(tptype);
//   newTP->set_orig_id(n_rank);
//   newTP->set_dest_id(n_rank);

//   DECARD_INFOMSG(1, "%s: Generating TP_%04d", decard_rt.hostname, tp_id);
//   decard_rt.add_TP(newTP);

//   tptype = END;
//   tp_id = 999;
//   // Create TP
//   newTP = new ThreadedProcedure();
//   newTP->set_opr(tp_id);
//   newTP->set_tptype(tptype);
//   newTP->set_orig_id(n_rank);
//   newTP->set_dest_id(n_rank);
//   DECARD_INFOMSG(1, "%s: Generating TP_%04d", decard_rt.hostname, tp_id);
//   decard_rt.add_TP(newTP);

//   // Start Runtime
//   decard_rt.run();
//   // Finalize Runtime
//   decard_rt.end();

//   // Finalize the MPI environment. No more MPI calls can be made after this
//   MPI_Finalize();
}