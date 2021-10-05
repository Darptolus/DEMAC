#include "decard.hpp"

using namespace decard;

int main(int argc, char *argv[]){

  Runtime decard_rt{};
  // Start Runtime
  decard_rt.run();
  // Finalize Runtime
  decard_rt.end();

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}