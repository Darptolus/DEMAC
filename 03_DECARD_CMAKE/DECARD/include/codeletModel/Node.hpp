/**
 * @file Node.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Node 
 * @todo Add copyright
 *
 * Includes the definition of the Node for each board of the cluster
 */

#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <omp.h>
#include <mpi.h>
#include <vector>
#include "NodeInterface.hpp"
#include <NCOM.hpp>
#include <NMGR.hpp>
#include <edarts.hpp>

namespace decard
{
  class Node_Intern: public Node
  {
  private:
    NCOM this_NCOM;
    NMGR this_NMGR;
    // thread_safe::deque<ThreadedProcedure*> INTPQ;
    // thread_safe::deque<ThreadedProcedure*> ONTPQ;
    // thread_safe::deque<ThreadedProcedure*> ISTPQ;
    // thread_safe::deque<ThreadedProcedure*> OSTPQ;
    control_q ICTRQ;
    control_q OCTRQ;
  public: 
    char node_name[HOST_NAME_MAX+1];
    Node_Intern(int w_rank, int w_size, AllNodes * a_nodes):
    Node(w_rank, w_size), 
    this_NCOM(a_nodes, this, &ICTRQ, &OCTRQ),
    this_NMGR(a_nodes, this, &ICTRQ, &OCTRQ){
      gethostname(node_name, HOST_NAME_MAX+1);
      // printf("NODENAME: %s \n", node_name);
    };
    ~Node_Intern(){};
    int run();
  };

  class Node_Extern: public Node
  {
  private:
    MPI_Request rcv_req;
    MPI_Status rcv_sts;
    int test_flagS;
    int test_flagR;
    int msg_box;
  public:
    Node_Extern(int w_rank, int w_size):Node(w_rank, w_size){};
    ~Node_Extern(){}
    int run();
    void set_msgbox(int a_msg){
      this->msg_box = a_msg;
    };
    MPI_Request * get_rreq(){return &rcv_req;}
    MPI_Status * get_rsts(){return &rcv_sts;}
    int * get_msgbox(){return &msg_box;}
  };

} // namespace decard
#endif /* NODE_H */