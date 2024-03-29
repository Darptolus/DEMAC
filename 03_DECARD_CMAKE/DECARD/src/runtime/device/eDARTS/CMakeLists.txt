################################################################################
#                                                                              #
# Copyright (c) 2011-2017, University of Delaware                              #
# All rights reserved.                                                         #
#                                                                              #
# Redistribution and use in source and binary forms, with or without           #
# modification, are permitted provided that the following conditions           #
# are met:                                                                     #
#                                                                              #
# 1. Redistributions of source code must retain the above copyright            #
# notice, this list of conditions and the following disclaimer.                #
#                                                                              #
# 2. Redistributions in binary form must reproduce the above copyright         #
# notice, this list of conditions and the following disclaimer in the          #
# documentation and/or other materials provided with the distribution.         #
#                                                                              #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS          #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT            #
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS            #
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE               #
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,         #
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,         #
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;             #
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER             #
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT           #
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN            #
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE              #
# POSSIBILITY OF SUCH DAMAGE.                                                  #
#                                                                              #
################################################################################

cmake_minimum_required(VERSION 3.10)
include("./cmake/CMakeColors.txt")

# In order to use e-gcc crosscompiler we use this
# taken from http://www.vtk.org/Wiki/CMake_Cross_Compiling
SET(CMAKE_SYSTEM_NAME Generic)

project(eDARTS VERSION 0.1 DESCRIPTION "Epiphany DARTS version")

message_bgreen( "=== Project eDARTS CMAKE ===" )

# epiphany variables
set(EPIPHANY_HOME /opt/adapteva/esdk)
set(ELDF_INTERNAL ${EPIPHANY_HOME}/bsps/current/internal.ldf)
set(ELDF_FAST ${EPIPHANY_HOME}/bsps/current/fast.ldf)
set(ELDF_EDARTS_CACHEMAN ${CMAKE_SOURCE_DIR}/include/assets/fast_cacheman_codelets.ldf)
set(ELDF_EDARTS ${CMAKE_SOURCE_DIR}/include/assets/darts.ldf)

# setting up the right compiler and linker
set(CMAKE_C_COMPILER ${EPIPHANY_HOME}/tools/e-gnu.armv7l/bin/e-gcc)
set(CMAKE_LINKER ${EPIPHANY_HOME}/tools/e-gnu/bin/e-ld)
set(EPIPHANY_OBJ_FILES "${EPIPHANY_HOME}/tools/e-gnu/epiphany-elf/lib/crt0.o ${EPIPHANY_HOME}/tools/e-gnu/lib/gcc/epiphany-elf/8.3.0/crti.o ${EPIPHANY_HOME}/tools/e-gnu/lib/gcc/epiphany-elf/8.3.0/crtbegin.o ${EPIPHANY_HOME}/tools/e-gnu/lib/gcc/epiphany-elf/8.3.0/crtend.o ${EPIPHANY_HOME}/tools/e-gnu/lib/gcc/epiphany-elf/8.3.0/crtn.o")

set(CMAKE_C_LINK_EXECUTABLE
"<CMAKE_LINKER> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> ${EPIPHANY_OBJ_FILES} -o <TARGET> <LINK_LIBRARIES>")

# Include directories
include_directories(include
                    include/assets
                    include/multithreading
                    include/runtime
                    include)

add_subdirectory( src )
# add_subdirectory( test )                     include/codeletModel

# We want to install in $HOME user directory
if( "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local" )
    message_yellow( "=> SETTING INSTALL TO HOME" )
    set (CMAKE_INSTALL_PREFIX $ENV{HOME} )
endif( "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local" )

message_yellow("=> Looking for dependencies:")

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

#lookig for gcc
find_library(GCC_LIB gcc PATH ${EPIPHANY_HOME}/tools/e-gnu/lib/gcc/epiphany-elf/8.3.0/)
if (GCC_LIB)
    message_green("\tlibgcc FOUND = ${GCC_LIB}")
else()
    error_red( "\tlibgcc NOT FOUND")
endif()

#Looking for libg
find_library(G_LIB g PATH ${EPIPHANY_HOME}/tools/e-gnu/epiphany-elf/lib/)

if (G_LIB)
    message_green("\tlibg FOUND = ${G_LIB}")
else()
    error_red( "\tlibg NOT FOUND")
endif()

#Looking for libc
find_library(C_LIB c PATH ${EPIPHANY_HOME}/tools/e-gnu/epiphany-elf/lib/)

if (C_LIB)
    message_green("\tlibc FOUND = ${C_LIB}")
else()
    error_red( "\tlibc NOT FOUND")
endif()

#Looking for libc
find_library(M_LIB m PATH ${EPIPHANY_HOME}/tools/e-gnu/epiphany-elf/lib/)

if (M_LIB)
    message_green("\tlibm FOUND = ${M_LIB}")
else()
    error_red( "\tlibm NOT FOUND")
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

#Looking for libe-hal
find_library(PTHREAD_LIB pthread PATH /usr/lib/arm-linux-gnueabihf/)

if (PTHREAD_LIB)
    message_green("\tlibpthread FOUND = ${PTHREAD_LIB}")
else()
    error_red( "\tlibpthread NOT FOUND")
endif()

#Compilation flags
option(DEBUG "DEBUG" ON)
if (DEBUG)
    message_yellow("=> DEBUG MODE ON!")
    set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-g -ffast-math -mfp-mode=round-nearest -fpic ")
else ()
    set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-ffast-math -mfp-mode=round-nearest -fpic ")
endif()

set(EPIPHANY_LIBS ${M_LIB} ${ELIB_LIB} ${GCC_LIB} ${G_LIB} ${C_LIB} ${EPIPHANY_LIB})

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -T ${ELDF_EDARTS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${ELDF_EDARTS} ${CMAKE_BINARY_DIR}/src/softcache/cachemanager.o")

#including eDARTS
message_bgreen("=> Starting eDARTS environment")

add_executable(e_darts.elf e_darts.c)

target_link_libraries(e_darts.elf ${EPIPHANY_LIBS}
                                  e_darts)

add_dependencies(e_darts.elf edarts_cachemanager)

# cd ~/DEMAC_nfs/debug/edarts
# cmake ../../DECARD/src/runtime/device/eDARTS/