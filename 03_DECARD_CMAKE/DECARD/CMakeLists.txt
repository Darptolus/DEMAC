cmake_minimum_required(VERSION 3.10)
include("./cmake/CMakeColors.txt")

# set the project name and version
project(DECARD VERSION 0.1 DESCRIPTION "Distributed runtime for the Codelet Model")

message_bgreen(" ==> DECARD <==")

# Set OpenMP flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# Epiphany variables
set(EPIPHANY_HOME /opt/adapteva/esdk)
set(ELDF_INTERNAL ${EPIPHANY_HOME}/bsps/current/internal.ldf)
set(ELDF_FAST ${EPIPHANY_HOME}/bsps/current/fast.ldf)
set(ELDF_EDARTS_CACHEMAN ${CMAKE_SOURCE_DIR}/eDARTS/include/assets/fast_cacheman_codelets.ldf)
set(ELDF_EDARTS ${CMAKE_SOURCE_DIR}/eDARTS/include/assets/darts.ldf)


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
else ()
  message_bgreen("RELEASE MODE COMPILATION")
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

## Check for ACCELERATOR
if (EPIPH)
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -le-hal -le-loader -lpthread")
  message_yellow("=> Looking for Epiphany:")
  #Looking for libe_lib
  find_library(ELIB_LIB e-lib PATH ${EPIPHANY_HOME}/tools/e-gnu/epiphany-elf/lib/)

  if (ELIB_LIB)
      message_green("\tlibe_lib FOUND = ${ELIB_LIB}")
  else()
      error_red( "\tlibe_lib NOT FOUND")
  endif()

  #Looking for libepiphany
  find_library(EPIPHANY_LIB epiphany PATH ${EPIPHANY_HOME}/tools/e-gnu/epiphany-elf/lib/)

  if (EPIPHANY_LIB)
      message_green("\tlibepiphany FOUND = ${EPIPHANY_LIB}")
  else()
      error_red( "\tlibepiphany NOT FOUND")
  endif()

  #Looking for libe-hal
  find_library(E_HAL_LIB e-hal PATH ${EPIPHANY_HOME}/tools/host/lib/)

  if (E_HAL_LIB)
      message_green("\tlibe-hal FOUND = ${E_HAL_LIB}")
  else()
      error_red( "\tlibe-hal NOT FOUND")
  endif()

  #Looking for libe-hal
  find_library(E_LOADER_LIB e-loader PATH ${EPIPHANY_HOME}/tools/host/lib/)

  if (E_LOADER_LIB)
      message_green("\tlibe-loader FOUND = ${E_LOADER_LIB}")
  else()
      error_red( "\tlibe-loader NOT FOUND")
  endif()

  #Looking for libpthread
  find_library(PTHREAD_LIB pthread PATH /usr/lib/arm-linux-gnueabihf/)

  if (PTHREAD_LIB)
      message_green("\tlibpthread FOUND = ${PTHREAD_LIB}")
  else()
      error_red( "\tlibpthread NOT FOUND")
  endif()

  include_directories(${EPIPHANY_HOME}/tools/host/include) 

endif (EPIPH)

#including DECARD
message_bgreen("=> Starting DECARD environment")

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

# target_link_libraries(my_mpi_target ${MPI_C_LIBRARIES})

message_green("=> Copying NodeList")
add_custom_command(
   OUTPUT nodeList
  #  COMMAND cp ../decard/nodeList nodeList
   COMMAND touch ThisIsATest.txt
   DEPENDS nodeList
   COMMENT "Copying NodeList"
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   VERBATIM)

add_subdirectory( src )
# add_subdirectory( tests )

add_executable(decard_main main_DECARD.cpp)

message_green("=> Linking Libraries")

target_link_libraries(decard_main PRIVATE OpenMP::OpenMP_CXX
                                  PRIVATE ${MPI_LIBRARIES}
                                  PRIVATE decard)

if (EPIPH)
  target_link_libraries(decard_main PRIVATE ${E_HAL_LIB}
                                    PRIVATE ${E_LOADER_LIB}
                                    PRIVATE ${PTHREAD_LIB})
endif (EPIPH)

message_green("=> END CMAKE <=")

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
# e-gcc -T /opt/adapteva/esdk/bsps/current/internal.ldf -O0 e00_nodeCheck.c -o e00_nodeCheck.elf -le-lib
# mpirun -machinefile nodeList -N 1 -H nopa08,nopa10 ./main.elf
# mpirun -machinefile nodeList -N 1 -np 2 ./decard_main.out 

# cmake ../DECARD
# cmake --build .
# ./decard_main.out

# cmake ../DECARD -DDEBUG=0 -DVERBOSE_MODE=1 -DEPIPH=1
