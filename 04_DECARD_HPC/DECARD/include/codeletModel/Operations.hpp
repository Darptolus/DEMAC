/**
 * @file Operations.hpp
 * @author Diego Roa
 * @date 29 Jan 2022
 * @brief Operations
 * @todo Add copyright
 *
 * Includes the definition of the operations for the runtime
 */

#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

namespace decard
{
  enum ops_type // Operation Type
  {
    N_Z, // Zero0
    N_A, // (X) Node ACK
    N_N, // (X) Node Not-ACK
    // Node status
    N_S, // Node Sending
    N_R, // Node Receiving
    N_D, // Node Done
    N_E, // Node Error
    // Workload Management
    TIV, // TP Invoke
    TSR, // (X) TP Send Request
    TRR, // (X) TP Receive Request
    TSE, // TP Send
    TRE, // TP Receive
    TDN, // TP Done (Continuation Codelet?)
    // Data Management
    DSR, // (X) Data Send Request
    DRR, // (X) Data Receive Request
    DSE, // Data Send 
    DRE, // (X) Data Receive 
  };
}
#endif /* OPERATIONS_HPP */

