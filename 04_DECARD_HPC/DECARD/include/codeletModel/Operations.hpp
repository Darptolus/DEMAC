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
    N_Z, // Zero
    N_A, // Node ACK
    N_N, // Node Not-ACK
    // Node status
    N_S, // Node Sending
    N_R, // Node Receiving
    N_D, // Node Done
    N_E, // Node Error
    // Workload Management
    TSR, // TP Send Request
    TRR, // TP Receive Request
    TSE, // TP Send
    TRE, // TP Receive
    TDN, // TP Done
    // Data Management
    DSR, // Data Send Request
    DRR, // Data Receive Request
    DSE, // Data Send 
    DRE, // Data Receive 
  };
}
#endif /* OPERATIONS_HPP */

