/**
 * @file Messages.cpp
 * @author Diego Roa
 * @date 29 Jan 2022
 * @brief Messages
 * @todo Add copyright
 *
 * Includes the definition of the messages for the runtime
 */

#include "Messages.hpp"
#include "Node.hpp"

using namespace decard;

int Message::get_orig_id()
{
  return node_orig->get_id();
}