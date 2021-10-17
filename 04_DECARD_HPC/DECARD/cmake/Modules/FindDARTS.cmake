################################################################################
#                                                                              #
# Copyright (c) 2011-2014, University of Delaware                              # 
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

#[=======================================================================[.rst:
FindDARTS
-------

Finds the DARTS library. The Delaware Adaptive Runtime System

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``darts::darts``
  The DARTS library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``DARTS_FOUND``
  True if the system has the Foo library.
``DARTS_VERSION``
  The version of the Foo library which was found.
``DARTS_INCLUDE_DIRS``
  Include directories needed to use Foo.
``DARTS_LIBRARIES``
  Libraries needed to link to Foo.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``DARTS_INCLUDE_DIR``
  The directory containing ``foo.h``.
``DARTS_LIBRARY``
  The path to the Foo library.

#]=======================================================================]


set(POSSIBLE_LIB_LOCATION /usr/local/darts)

find_path(DARTS_INCLUDE_DIR "darts.h" 
  ${POSSIBLE_LIB_LOCATION}/include
)

find_library(DARTS_LIB NAMES darts
  PATHS ${POSSIBLE_LIB_LOCATION}/lib
)
find_library(CODELET_LIB NAMES codelet
  PATHS ${POSSIBLE_LIB_LOCATION}/lib
)
find_library(AMM_LIB NAMES amm 
  PATHS ${POSSIBLE_LIB_LOCATION}/lib
)
find_library(COMMON_LIB NAMES common
  PATHS ${POSSIBLE_LIB_LOCATION}/lib
)
find_library(SCHEDUELR_LIB NAMES scheduler  
  PATHS ${POSSIBLE_LIB_LOCATION}/lib
)
find_library(SCHEDUELR_LIB NAMES threadlocal 
  PATHS ${POSSIBLE_LIB_LOCATION}/lib
)

if (DARTS_LIB AND CODELET_LIB AND AMM_LIB AND COMMON_LIB AND SCHEDUELR_LIB AND SCHEDUELR_LIB)
  set(DARTS_LIBRARIES ${DARTS_LIB} ${CODELET_LIB} ${AMM_LIB} ${COMMON_LIB} ${SCHEDUELR_LIB} ${SCHEDUELR_LIB})
endif(DARTS_LIB AND CODELET_LIB AND AMM_LIB AND COMMON_LIB AND SCHEDUELR_LIB AND SCHEDUELR_LIB)

set(VERSION_DARTS 0.1)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DARTS DEFAULT_MSG
  DARTS_LIBRARIES
  DARTS_INCLUDE_DIR
  VERSION_DARTS 0.1)

if(DARTS_FOUND)
  set( DARTS_INCLUDE_DIR ${DARTS_INCLUDE_DIR} )
  set( DARTS_LIBRARIES ${DARTS_LIBRARIES} )
endif(DARTS_FOUND)

