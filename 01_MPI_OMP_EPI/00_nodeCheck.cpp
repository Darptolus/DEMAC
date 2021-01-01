/*
 * Author: Noémien Kocher
 * Date: january 2016
 * Licence: MIT
 * Purpose:
 *   Computes a game of life where each core is an independent cell.
 *   The main program reads for a certain number of iterations the status of the
 *   cells and displays it. Outputs at the end the number of iterations for each
 *   cell (eCore) and the status of the sticky overflow flag.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <mpi.h>

#include <e-loader.h>
#include <e-hal.h>  // Epiphany Hardware Abstraction Layer
                    // functionality for communicating with epiphany chip when
                    // the application runs on a host, typically the ARM µp

#define BUFOFFSET (0x01000000)  // SDRAM is at 0x8f00'0000,
                                // offset in e_read starts at 0x8e00'0000

unsigned rows, cols, i, j, ncores, row, col;

/*
 * Init the epiphany platform
 */
 
void init_epiphany(e_platform_t * platform) {
  e_init(NULL);
  e_reset_system();
  e_get_platform_info(platform);
}

/*
 * Create the workgroup and load programs into it
 */
int init_workgroup(e_epiphany_t * dev) {
  e_return_stat_t result;
  e_open(dev, 0, 0, rows, cols); // Create an epiphany cores workgroup
  e_reset_group(dev);
  // load programs into cores workgroup, do not execute it immediately
  result = static_cast<e_return_stat_t>(e_load_group("e00_nodeCheck.elf", dev, 0, 0, rows, cols, E_FALSE));
  if(result != E_OK) {
    printf("Error Loading the Epiphany Application %i\n", result);
  }
  e_start_group(dev);
}

/*
 * Main entry
 */
int main(int argc, char * argv[]) {

MPI_Init(NULL, NULL);
		int world_size;
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		int world_rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

		char node_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Get_processor_name(node_name, &name_len);
	
	int e_flag_n[4];
	int e_flag_t[4][world_size];

	struct timeval start, end;
  
  // Arguments handling

  e_platform_t platform;  // platform infos
  e_epiphany_t dev;       // provides access to cores workgroup
  e_mem_t emem;           // shared memory buffer

  init_epiphany(&platform);

  rows = platform.rows;
  cols = platform.cols;
  ncores = rows * cols;
  uint32_t result[ncores];     // to store the results, size of cores
  // allocate a space to share data between e_cores and here
  // offset starts from 0x8e00'0000
  // sdram (shared space) is at 0x8f00'0000
  // so 0x8e00'0000 + 0x0100'0000 = 0x8f00'0000
  e_alloc(&emem, BUFOFFSET, ncores*sizeof(uint32_t));
  init_workgroup(&dev);


gettimeofday(&start, NULL);


#pragma omp parallel num_threads(2)
{
	int core_id = omp_get_thread_num();
  int core_tot = omp_get_num_threads();

  if (core_id == 1){

    // we read from the allocated space and store it to the result array
      usleep(100);
      e_read(&emem, 0, 0, 0x0, &result, ncores * sizeof(uint32_t)); // reads what's ben put in buffer
      for(row = 0; row < rows; row++) {
        for(col = 0; col < cols; col++) {
          fprintf(stdout, "[%i]", result[row*cols+col]);
        }
      }
      fprintf(stdout, "\n");
      fflush(stdout);
    
    
    e_close(&dev);
  }

  e_flag_n[core_id] = 1; // 1 = ok
	e_flag_n[core_id+2] = world_rank; // 1 = ok

	#pragma omp barrier


} // end pragma omp

gettimeofday(&end, NULL);

printf("%s: %lf us\n", node_name, (((end.tv_sec * 1000000 + end.tv_usec)
				- (start.tv_sec * 1000000 + start.tv_usec)))*1.0);

gettimeofday(&start, NULL);
fflush(stdout);

MPI_Barrier(MPI_COMM_WORLD);

MPI_Gather(&e_flag_n[0], 1, MPI_INT, &e_flag_t[0][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Gather(&e_flag_n[1], 1, MPI_INT, &e_flag_t[1][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Gather(&e_flag_n[2], 1, MPI_INT, &e_flag_t[2][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Gather(&e_flag_n[3], 1, MPI_INT, &e_flag_t[3][world_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);

MPI_Barrier(MPI_COMM_WORLD);

gettimeofday(&end, NULL);

if (world_rank == 0){
	for(int j = 0; j< sizeof(e_flag_n)/sizeof(int); j++){
		for(int i = 0; i< world_size; i++){
			//printf("%d:\n",i);		
			printf("[%d]", e_flag_t[j][i]);
		}
		printf("\n");
	}
	printf("Time 2: %lf us\n", (((end.tv_sec * 1000000 + end.tv_usec)
				- (start.tv_sec * 1000000 + start.tv_usec)))*1.0);
}

MPI_Finalize();

  return 0;
}
