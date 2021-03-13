/**
 * @file runtime.cpp
 * @author Diego Roa
 * @date 31 Dec 2020
 * @brief Runtime
 * @todo Add copyright
 *
 * Includes the definition of the Runtime
 */

#include "runtime.hpp"

using namespace decard;

// void print_rt_mode(runtime_mode rtype) {
//   printf("RUNTIME ");
//   switch(rtype) {
//     case R_DONE:
//         printf("DONE\n");
//         break;
//     case R_EXEC:
//         printf("EXEC\n");
//         break;
//     case R_IDLE:
//         printf("IDLE\n");
//         break;
//     case R_UNDEF:
//         printf("UNDEF\n");
//         break;
//     default:
//         printf("Invalid Mode\n");
//   }
// }

int Runtime::run()
{
  // rt_mode = ALL_RECV;
  // // ToDo: Set for Debug mode only 
  // print_rt_mode(rt_mode);
  
  DECARD_AM.run();

  // do{
  //   usleep(1000000);
  //   // ToDo: Set for Debug mode only 
  //   print_rt_mode(rt_mode);
  //   // Get state
  //   machine_state m_state = M_DONE;
  //   DECARD_AM.set_state(m_state);
  // }while(DECARD_AM.get_state() != M_DONE);

  // rt_mode = ALL_DONE;
  // // ToDo: Set for Debug mode only 
  // print_rt_mode(rt_mode);
  
   
  
  return 0;
}

// int Runtime::run(runtime_mode a_rt_mode)
// {
//   rt_mode = a_rt_mode;
//   // ToDo: Set for Debug mode only 
//   print_rt_mode(rt_mode);


//   DECARD_AM.start_AM();
  
//   return 0;
// }

int Runtime::end_rt()
{
  
  return 0;
}

