/*
 * Author: Noémien Kocher
 * Date: january 2016
 * Licence: MIT
 * Purpose:
 *   This file is run by each eCore, it represents the life of a cell. Until it
 *   is stopped, loops over and check its neighbor so as to update its status.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <e-lib.h> // Epiphany cores library

char swap[8] SECTION(".text_bank2");
volatile uint32_t *result;

int main(void) {

  unsigned core_row, core_col,
           group_rows, group_cols,
           core_num;
  
  core_row = e_group_config.core_row;
  core_col = e_group_config.core_col;
  group_rows = e_group_config.group_rows;
  group_cols = e_group_config.group_cols;

  core_num = core_row * group_cols + core_col;

  // our swap, could (should) put it in a structure
  // starts at the beginning of sdram
  result  = (volatile uint32_t*) (0x8f000000 + 0x4*core_num); // writing to external memory, writing 4bytes
 
  *result = core_num; // store result
    
  
}
