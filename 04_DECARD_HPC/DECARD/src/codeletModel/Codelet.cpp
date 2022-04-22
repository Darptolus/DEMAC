/**
 * @file Codelet.cpp
 * @author Diego Roa
 * @date 14 Oct 2021
 * @brief Codelet
 * @todo Add copyright
 *
 * Includes the definition of codelets
 */

#include "Codelet.hpp"
#include "ThreadedProcedure.hpp"

using namespace decard;

void Codelet::decDep()
{
  if(!s_slot.decCount()){
    // Dependencies count reach zero
    this->stus_enbl(); // Set status ENBL
  }
}

int Codelet::dec_nexec()
{ 
  // Decrease counter for TP
  t_TP->dec_cdnexec();
  return (Atomics::fetchSub(n_exec, 1));
}

int Codelet::get_tpid()
{
  // Get TP ID
  return (t_TP->get_id());
}

int Codelet::get_tpid_full()
{
  // Get TP full ID
  return (t_TP->get_id_full());
}

// void Codelet::resetCodelet()
