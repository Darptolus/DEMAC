#include <mpi.h>
#include <stdio.h>
#include "decard.hpp"

using namespace decard;

class cd_01 : public Codelet
{
  public:
  cd_01(ThreadedProcedure * myTP, codelet_status stat, uint32_t dep, uint32_t res):
  Codelet(myTP, stat, dep, res){}

  virtual void fire(void);
};

//This codelet is the continuation of the intiated split phase
class cd_02 : public Codelet
{
public:
  cd_02(ThreadedProcedure * myTP, codelet_status stat, uint32_t dep, uint32_t res):
  Codelet(myTP, stat, dep, res){}

  virtual void fire(void);
};

class cd_03 : public Codelet
{
public:
  cd_03(ThreadedProcedure * myTP, codelet_status stat, uint32_t dep, uint32_t res):
  Codelet(myTP, stat, dep, res){}

  virtual void fire(void);
};


//This is the Hello World threaded proceduer
class HelloWorld : public ThreadedProcedure
{
public:
    cd_01 HW;
    cd_02 Check;
    //This is who to signal when complete
    Codelet * toSignal;
    
    //Constructor
    // HelloWorld(int n, int * res, Codelet * toSig):
    // ThreadedProcedure(),
    // num(n),
    // x(0),
    // y(0),
    // result(res),
    // check(0,0,this,SHORTWAIT),
    // adder(2,2,this,LONGWAIT),
    // toSignal(toSig) 
    // {  
    //     add(&check); 
    // }
    
    //~fib(void)
    //{
    //    std::cout << "dieing "<< num << std::endl;
    //}
    
};

void
cd_01::fire(void)
{
    // fib * myFib = static_cast<fib*>(myTP_);
    
    // //std::cout << "check " << myFib->num << std::endl;
    
    // if(myFib->num<2)
    // {
    //     (*myFib->result) = myFib->num;
    //     myFib->toSignal->decDep();
    // }
    // else
    // {
    //     invoke<fib>(myFib,myFib->num-1,&myFib->x,&myFib->adder);
    //     invoke<fib>(myFib,myFib->num-2,&myFib->y,&myFib->adder);
    // }
}
 
void 
cd_02::fire(void)
{ 
    // fib * myFib = static_cast<fib*>(myTP_);    
    // //std::cout << "add " << myFib->num << std::endl;
    // (*myFib->result) = myFib->x + myFib->y;
    // myFib->toSignal->decDep();
}

int main(int argc, char** argv) {
  // // Initialize the MPI environment. The two arguments to MPI Init are not
  // // currently used by MPI implementations, but are there in case future
  // // implementations might need the arguments.
  // MPI_Init(NULL, NULL);

  // // Get the number of processes
  // int world_size;
  // MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // // Get the rank of the process
  // int world_rank;
  // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // // Get the name of the processor
  // char processor_name[MPI_MAX_PROCESSOR_NAME];
  // int name_len;
  // MPI_Get_processor_name(processor_name, &name_len);

  // // Print off a hello world message
  // printf("Hello world from processor %s, rank %d out of %d processors\n",
  //        processor_name, world_rank, world_size);

  // // Finalize the MPI environment. No more MPI calls can be made after this
  // MPI_Finalize();


  Runtime decard{};
  int w_size, n_rank, x, y, oopr;
  ThreadedProcedure * newTP;

  n_rank = decard.get_worldrank();
  w_size = decard.get_worldsize();
  y = 1;
  // Generate TPs
  do{
    if (x==y){
      // End TP
      oopr = 999;
    }else{
      oopr = 1;
    }      
    // Generate TP
    newTP = new ThreadedProcedure();
    newTP->set_opr(oopr);
    newTP->set_orig(n_rank);
    newTP->set_dest(n_rank);
    DECARD_INFOMSG(1, "%s: Generating M_%04d", decard.get_nodename(), oopr);
    decard.add_TP(newTP);
    ++x;
  }while(x <= y);

  // Start Runtime
  decard.run();

  // Finalize Runtime
  decard.end();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}