/**
 * @file Messages.hpp
 * @author Diego Roa
 * @date 29 Jan 2022
 * @brief Messages
 * @todo Add copyright
 *
 * Includes the definition of the messages for the runtime
 */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "Operations.hpp"
#include "NodeInterface.hpp"

namespace decard
{
  //This is a forward declaration since there is a circular dependence
  class Node;  
  struct Msg1{
    int m_id;
    ops_type m_opr;
    int m_size;
    int m_data;
  };
  class Message
  {
  private:
    int id;
    ops_type op_type;
    Node * node_orig;
    Node * node_dest;
    ThreadedProcedure * TP;
    int data;
    int arg_size;
  public:
    Message(
      ops_type a_optype, int a_data):
      op_type(a_optype), data(a_data){}
    Message(
      ops_type a_optype, Node * a_node_orig):
      op_type(a_optype), node_orig(a_node_orig){}
    Message(
      ops_type a_optype, Node * a_node_orig, Node * a_node_dest):
      op_type(a_optype), node_orig(a_node_orig), node_dest(a_node_dest){}
    ~Message(){}
    ops_type get_opr(){return op_type;}
    int get_orig_id();
    int * get_msg1(){
      Msg1 * msg_1= new Msg1;
      msg_1->m_id = id;
      msg_1->m_opr = op_type;
      msg_1->m_size = arg_size;
      msg_1->m_data = id;
      int * p_msg_1 = (int *) msg_1;
      return p_msg_1;
    }
  };
  
  // Message Queue
  typedef thread_safe::deque<Message*> cl_q;
}
#endif /* MESSAGES_HPP */

