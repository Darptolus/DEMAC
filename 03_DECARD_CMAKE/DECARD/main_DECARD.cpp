#include <stdlib.h>
#include <stdio.h>
#include "decard.hpp"
#include <mpi.h>

int main(int argc, char *argv[]){

  start_DECARD();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();

}