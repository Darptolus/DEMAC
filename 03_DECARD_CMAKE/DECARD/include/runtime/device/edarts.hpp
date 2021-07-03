/**
 * @file edarts.hpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief eDARTS
 * @todo Add copyright
 *
 * Includes the definition of the accelerator for each node of the cluster
 */

#ifndef EDARTS_HPP
#define EDARTS_HPP

#include <stdio.h>
#include <stdlib.h>

#include <e-loader.h>
#include <e-hal.h>  // Epiphany Hardware Abstraction Layer
                    // functionality for communicating with epiphany chip when
                    // the application runs on a host, typically the ARM µp

#define BUFOFFSET (0x01000000)  // SDRAM is at 0x8f00'0000,
                                // offset in e_read starts at 0x8e00'0000

namespace decard
{
  class eDARTS
  {
    private:
      unsigned rows, cols, i, j, ncores, row, col;
      e_platform_t platform;
      e_epiphany_t dev;       // provides access to cores workgroup
      e_mem_t emem;           // shared memory buffer
      e_return_stat_t result_e;
      uint32_t result[16];     // to store the results, size of cores
      // allocate a space to share data between e_cores and here
      // offset starts from 0x8e00'0000
      // sdram (shared space) is at 0x8f00'0000
      // so 0x8e00'0000 + 0x0100'0000 = 0x8f00'0000
    public:
      eDARTS(){};
      ~eDARTS(){};
      unsigned get_rows(){ return rows;};
      unsigned get_cols(){ return cols;};
      
      void init(){
        // printf("INITIALIZING eDARTS\n");
        e_init(NULL);
        e_reset_system();
        e_get_platform_info(&platform);
        rows = platform.rows;
        cols = platform.cols;
        ncores = rows * cols;
      };
      int run();
      void p_res();
      void end();
  };
}
#endif /* eDARTS_HPP */