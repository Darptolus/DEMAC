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

namespace decard
{
  class Message
  {
  private:
    ops_type op_type;
    Node * node_orig;
    Node * node_dest;
    ThreadedProcedure * TP;
    int arg_size;
  public:
    Message(
      ops_type a_optype, Node * a_node_orig):
      op_type(a_optype), node_orig(a_node_orig){};
    Message(
      ops_type a_optype, Node * a_node_orig, Node * a_node_dest):
      op_type(a_optype), node_orig(a_node_orig), node_dest(a_node_dest){};
    ~Message(){};
    ops_type get_opr(){return op_type;};
  };
}
#endif /* MESSAGES_HPP */

