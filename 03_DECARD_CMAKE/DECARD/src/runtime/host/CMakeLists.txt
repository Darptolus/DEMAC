set( decard_runtime_host_src
     NCOM.cpp
     NMGR.cpp
     )

set( decard_runtime_host_inc
     ${CMAKE_SOURCE_DIR}/include/runtime/host/NCOM.hpp
     ${CMAKE_SOURCE_DIR}/include/runtime/host/NMGR.hpp
     )

add_library( decard_runtime_host ${decard_runtime_host_src} ${decard_runtime_host_inc})

set_target_properties(decard_runtime_host PROPERTIES LINKER_LANGUAGE CXX
                                                     PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/runtime//host/NCOM.hpp ${CMAKE_SOURCE_DIR}/include/runtime//host/NGMR.hpp")

# install(TARGETS decard_runtime_host
#         EXPORT decardibraryDepends
#         ARCHIVE DESTINATION decard/lib/             COMPONENT bin
#         PUBLIC_HEADER DESTINATION decard/include/   COMPONENT dev)