cmake_minimum_required(VERSION 3.10)
include("./cmake/CMakeColors.txt")

# set the project name and version
project(DECARD VERSION 0.1 DESCRIPTION "Distributed runtime for the Codelet Model")
enable_language(Fortran)
message_bgreen(" ==> DECARD <==")

# Set OpenMP flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -std=cxx14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -std=c++11 -fPIC")

# # Epiphany variables
# set(EPIPHANY_HOME /opt/adapteva/esdk)
# set(ELDF_INTERNAL ${EPIPHANY_HOME}/bsps/current/internal.ldf)
# set(ELDF_FAST ${EPIPHANY_HOME}/bsps/current/fast.ldf)
# set(ELDF_EDARTS_CACHEMAN ${CMAKE_SOURCE_DIR}/eDARTS/include/assets/fast_cacheman_codelets.ldf)
# set(ELDF_EDARTS ${CMAKE_SOURCE_DIR}/eDARTS/include/assets/darts.ldf)


# Include directories
include_directories(include
                    include/runtime
                    include/runtime/host
                    include/runtime/device
                    include/codeletModel
                    include/assets
                    include)

## Check for DEBUG MODE
if (DEBUG)
  message_bgreen("DEBUG MODE COMPILATION")
  set(CMAKE_BUILD_TYPE Debug)
  enable_testing()
  add_subdirectory (tests) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
  ## Add Clang stuff here
else ()
  message_bgreen("RELEASE MODE COMPILATION")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")  
  set(CMAKE_BUILD_TYPE Release)
endif (DEBUG)

## Check for VERBOSE EXECUTION MODE
if (VERBOSE_MODE)
  message_yellow("=> DECARD VERBOSE MODE ENABLED")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERBOSE_MODE=${VERBOSE_MODE}")
  if (FNAME)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFNAME=${FNAME}")  
  endif (FNAME)
endif (VERBOSE_MODE)

## Check for PROFILE EXECUTION MODE
if (PROFILE_MODE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPROFILE_MODE=${PROFILE_MODE}")
endif (PROFILE_MODE)

if (DARTS)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")
  cmake_policy(PUSH)
  cmake_policy(SET CMP0074 NEW) # Allow to use DARTS_ROOT without having to explicitely defined it (envarimental variable)
    find_package(DARTS)
  cmake_policy(POP)

  if (DARTS_FOUND)
    message_yellow("Building with the cool DARTS")
  else ()
    message_red("Couldn't find DARTS")
  endif (DARTS_FOUND)
endif(DARTS)

# ## Check for ACCELERATOR
# if (EPIPH)
#   # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -le-hal -le-loader -lpthread")
#   message_yellow("=> Looking for Epiphany:")
#   #Looking for libe_lib
#   find_library(ELIB_LIB e-lib PATH ${EPIPHANY_HOME}/tools/e-gnu/epiphany-elf/lib/)

#   if (ELIB_LIB)
#       message_green("\tlibe_lib FOUND = ${ELIB_LIB}")
#   else()
#       error_red( "\tlibe_lib NOT FOUND")
#   endif()

#   #Looking for libepiphany
#   find_library(EPIPHANY_LIB epiphany PATH ${EPIPHANY_HOME}/tools/e-gnu/epiphany-elf/lib/)

#   if (EPIPHANY_LIB)
#       message_green("\tlibepiphany FOUND = ${EPIPHANY_LIB}")
#   else()
#       error_red( "\tlibepiphany NOT FOUND")
#   endif()

#   #Looking for libe-hal
#   find_library(E_HAL_LIB e-hal PATH ${EPIPHANY_HOME}/tools/host/lib/)

#   if (E_HAL_LIB)
#       message_green("\tlibe-hal FOUND = ${E_HAL_LIB}")
#   else()
#       error_red( "\tlibe-hal NOT FOUND")
#   endif()

#   #Looking for libe-hal
#   find_library(E_LOADER_LIB e-loader PATH ${EPIPHANY_HOME}/tools/host/lib/)

#   if (E_LOADER_LIB)
#       message_green("\tlibe-loader FOUND = ${E_LOADER_LIB}")
#   else()
#       error_red( "\tlibe-loader NOT FOUND")
#   endif()

#   #Looking for libpthread
#   find_library(PTHREAD_LIB pthread PATH /usr/lib/arm-linux-gnueabihf/)

#   if (PTHREAD_LIB)
#       message_green("\tlibpthread FOUND = ${PTHREAD_LIB}")
#   else()
#       error_red( "\tlibpthread NOT FOUND")
#   endif()

#   include_directories(${EPIPHANY_HOME}/tools/host/include) 

# endif (EPIPH)

#including DECARD
message_bgreen("=> Starting DECARD environment")

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

# if(NOT MPI_FOUND) 
#   error_red( "\tMPI NOT FOUND")
# endif()

include_directories(${MPI_INCLUDE_PATH})

# target_link_libraries(my_mpi_target ${MPI_C_LIBRARIES})

# message_green("=> Copying NodeList")
# add_custom_command(
#    OUTPUT nodeList
#   #  COMMAND cp ../decard/nodeList nodeList
#    COMMAND touch ThisIsATest.txt
#    DEPENDS nodeList
#    COMMENT "Copying NodeList"
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#    VERBATIM)

add_subdirectory( src )
# add_subdirectory( tests )
add_subdirectory( apps )

add_executable(decard_main main_DECARD.cpp)

message_green("=> Linking Libraries")

target_link_libraries(decard_main PRIVATE OpenMP::OpenMP_CXX
                                  PRIVATE ${MPI_LIBRARIES}
                                  PRIVATE decard)

# if (EPIPH)
#   target_link_libraries(decard_main PRIVATE ${E_HAL_LIB}
#                                     PRIVATE ${E_LOADER_LIB}
#                                     PRIVATE ${PTHREAD_LIB})
# endif (EPIPH)

message_green("=> END CMAKE <=")

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
# e-gcc -T /opt/adapteva/esdk/bsps/current/internal.ldf -O0 e00_nodeCheck.c -o e00_nodeCheck.elf -le-lib
# mpirun -machinefile nodeList -N 1 -H nopa08,nopa10 ./main.elf
# mpirun -machinefile nodeList -N 1 -np 2 ./decard_main.out 

# cmake ../DECARD
# cmake --build .
# ./decard_main.out

# cmake ../DECARD -DDEBUG=0 -DVERBOSE_MODE=1 -DEPIPH=1

## CAVINESS
# vpkg_require cmake/3.19.7 libfabric/1.9.0 openmpi/4.1.1 git/2.29.3 gcc/10.1.0
# cmake ../DECARD -DDEBUG=0 -DVERBOSE_MODE=1

# vpkg_require cmake/3.11.3 libfabric/1.9.0 openmpi/4.0.2 git/2.29.3 gcc/9.2.0 intel/2020u2
# N_NODES=2; sbatch -N $N_NODES -n $N_NODES openmpi_00.qs
# N_NODES=2; sbatch -N $N_NODES -n $N_NODES openmpi_decard.qs

# DARTS
# cmake ../ -DHWLOC_ROOT=/work/ececis_research/CAPSL/soft/hwloc-2.4.0 -DCUDA=OFF
# make -j
# make install