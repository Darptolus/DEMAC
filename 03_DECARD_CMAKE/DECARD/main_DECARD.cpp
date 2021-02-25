#include "decard.hpp"

using namespace decard;

int main(int argc, char *argv[]){

  Runtime decard_rt{};
  decard_rt.start_rt();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();

}