/**
 * @file edarts.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief eDARTS 
 * @todo Add copyright
 *
 * Includes the definition of the accelerator for each board of the cluster
 */
#include "edarts.hpp"

using namespace decard;

int eDARTS::run(){
  e_alloc(&emem, BUFOFFSET, 16*sizeof(uint32_t));
  e_open(&dev, 0, 0, get_rows(), get_cols()); // Create an epiphany cores workgroup
  e_reset_group(&dev);
  // load programs into cores workgroup, do not execute it immediately
  result_e = static_cast<e_return_stat_t>(e_load_group("edarts.elf", &dev, 0, 0, get_rows(), get_cols(), E_FALSE));
  if(result_e != E_OK) {
    printf("Error Loading the Epiphany Application %i\n", result_e);
  }
  e_start_group(&dev);
}

void eDARTS::p_res(){
  e_read(&emem, 0, 0, 0x0, result, 16 * sizeof(uint32_t)); // reads what's ben put in buffer
  // printf(">>>> emem: %u", reinterpret_cast<uint>(emem));
  for(row = 0; row < get_rows(); row++) {
    for(col = 0; col < get_cols(); col++) {
      fprintf(stdout, "[%i]", result[row*get_cols()+col]);
    }
  }
  fprintf(stdout, "\n");
  fflush(stdout);
}

void eDARTS::end(){
  e_close(&dev);
}