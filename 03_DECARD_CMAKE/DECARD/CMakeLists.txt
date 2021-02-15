cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(DECARD VERSION 0.1 DESCRIPTION "Distributed runtime for the Codelet Model")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

include_directories(include
                    include/runtime
                    include/runtime/host
                    include/runtime/device
                    include/codeletModel
                    include/assets
                    include)

add_subdirectory( src )
# add_subdirectory( tests )

#including DECARD
message("=> Starting DECARD environment\n")

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

# target_link_libraries(my_mpi_target ${MPI_C_LIBRARIES})

message("=> Copying NodeList\n")
add_custom_command(
   OUTPUT nodeList
  #  COMMAND cp ../decard/nodeList nodeList
   COMMAND touch ThisIsATest.txt
   DEPENDS nodeList
   COMMENT "Copying NodeList"
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   VERBATIM)

add_executable(decard_main main_DECARD.cpp)

message("=> Linking Libraries!!!\n")

target_link_libraries(decard_main PRIVATE OpenMP::OpenMP_CXX
                                  PRIVATE ${MPI_LIBRARIES}
                                  PRIVATE decard)

message("=> END MAIN CMAKE <=\n")

# set( decard_src
#      decard_runtime_host_src
#      )

# set( decard_inc
#      decard_runtime_host_inc
#      )

# add_library( decard SHARED ${decard_src} ${decard_inc})


# set_target_properties(decard PROPERTIES VERSION ${PROJECT_VERSION}
#                                         PUBLIC_HEADER include/decard.hpp)

# configure_file(decard.pc.in decard.pc @ONLY)

# target_include_directories(decard PRIVATE .)

# target_include_directories(mylib PRIVATE include)
# target_include_directories(mylib PRIVATE src)

# include(GNUInstallDirs)

# set( decard_src 
#      ${CMAKE_SOURCE_DIR}/src)

# set( decard_inc 
#      ${CMAKE_SOURCE_DIR}/include)

# install(TARGETS decard
#         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#         PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


# add_library()


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