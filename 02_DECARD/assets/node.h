
#include "Atomics.h"

#ifndef TPQ_SIZE
#define TPQ_SIZE 10
#endif

#ifndef MIN_TP_NUM
#define MIN_TP_NUM 1
#endif

#ifndef MAX_TP_NUM
#define MAX_TP_NUM 0
#endif

typedef void (*FncPtrType)(void);
enum comm_mode
{
  WAIT_R,
  WAIT_S,
  RECEIVE,
  SEND
};

class nodeComm
{
  public:
    comm_mode n_mode;
    int Msg_Rec; // Message
    bool T_flag; // ToBeDeleted used to print message
    bool R_set; // Not_Set = 0, Set = 1 
    int R_rec; // Not_Succesful_Rec = 0, Succesful_Rec = 1
};

// Define node actors
class ThreadedProcedure{
  protected:
	/*
	 * Variable: ref_
	 * ref_ says when the TP is done
	*/
    unsigned int ref_;
  public:
    int parent_n;
    int child_n;
    int opr_id;
    ThreadedProcedure * parentTP_;
    ThreadedProcedure(void);
    //ThreadedProcedure(unsigned int num, ThreadedProcedure * parentTP);
    //Destructor
    virtual ~ThreadedProcedure();
    
    bool decRef (void);
    void incRef (void);
    bool zeroRef (void);
    bool checkParent(void);
    /*
    * Method: addCodelet
    * Adds a codelet to the TP's list
    */
    //void add (Codelet * toAdd);
    // int ID = 0;
    // int opr_code = 0;
    // // Args
    // int set = 0; // 0 = void, 1=ready_out
    // int parent = 0;
    //int child = 0;
};

//Make the defualt reference count 1 so when stealing the TP will not be deleted prematurely
ThreadedProcedure::ThreadedProcedure(void):
ref_(1){ }
//parentTP_(myThread.tempParent){ }

//This is for paraFor loop
//ThreadedProcedure::ThreadedProcedure(unsigned int num, ThreadedProcedure * parentTP):
//ref_(num),
//parentTP_(parentTP){ }

ThreadedProcedure::~ThreadedProcedure(void)
{
    //We are deleting the TP
    //Does the TP have a parent
    if(parentTP_)
    {
        //Make sure he isn't a serial loop by checking if the parent has a parent
        if(parentTP_->checkParent())
        {
            //If he does lets dec the parent's counter and see if the parent has no more outstanding objects
            if(parentTP_->decRef())
            {
                delete parentTP_;
            }
        }
    }
}

bool 
ThreadedProcedure::decRef(void)
{
    return (1==Atomics::fetchSub(ref_, 1U));
}

void 
ThreadedProcedure::incRef(void)
{
    Atomics::fetchAdd(ref_, 1U);
}

bool
ThreadedProcedure::zeroRef(void)
{
    return (!ref_);
}

bool 
ThreadedProcedure::checkParent(void)
{
    if(parentTP_==NULL)
        return false;
    if(parentTP_==this)
        return false;
    return true;
    //return (parentTP_!=this);
}

/*void 
ThreadedProcedure::add(Codelet * toAdd)
{
    //check on where to set
    if(toAdd->codeletReady())
    {
        Atomics::fetchAdd(ref_, 1U);
        myThread.threadTPsched->pushCodelet(toAdd);
    }
}
*/


/* 
class nodeMngr
{
public:
  void tpScheduler();
  
  int nodeID;
  int tpNum;
}



class SUnit
{
  void scheduler();
}

class EUnit
{
  void executer();
}

void nodeMngr::tpQueue()
{
  for 
}
*/