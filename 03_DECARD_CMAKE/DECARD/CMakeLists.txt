cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(DECARD VERSION 0.1)

# include_directories(include
#                     include/assets
#                     include/runtime
#                     include/runtime/host
#                     include/runtime/device
#                     include/codeletModel
#                     include)

# add_subdirectory( src )
# add_subdirectory( tests )

#including DECARD
message("=> Starting DECARD environment\n")

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

add_executable(decard_main.out main_DECARD.cpp)
# add_library()

target_link_libraries(decard_main.out PRIVATE OpenMP::OpenMP_CXX
                                      PRIVATE ${MPI_LIBRARIES})

# target_link_libraries(my_mpi_target ${MPI_C_LIBRARIES})

# add_custom_command(
#    OUTPUT decard_main.out
#    COMMAND g++ main_DECARD.cpp ${EXTERNAL_FLAGS} -o decard_main.out
#    DEPENDS main_DECARD.cpp
#    COMMENT "CROSS-COMPILING HOST FILE DECARD "
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#    VERBATIM
# )

# mpic++ -I./ -I./assets -std=c++11 -fopenmp -o main.elf main.cpp -I /opt/adapteva/esdk/tools/host/include -L /opt/adapteva/esdk/tools/host/lib -le-hal -le-loader -lpthread
# mpirun -machinefile nodeList -N 1 -H nopa08,nopa10 ./main.elf
# mpirun -machinefile nodeList -N 1 -np 2 ./decard_main.out 

# cmake ../DECARD
# cmake --build .
# ./decard_main.out