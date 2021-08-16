set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -T ${ELDF_EDARTS_CACHEMAN}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${ELDF_EDARTS_CACHEMAN} ${CMAKE_BINARY_DIR}/src/softcache/cachemanager.o")

# set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -T ${ELDF_EDARTS_CACHEMAN}")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${ELDF_EDARTS_CACHEMAN} ${CMAKE_BINARY_DIR}/eDARTS/src/softcache/cachemanager.o")

set( e_darts_multithreading_src
     e_darts_mutex.c
     )

set( e_darts_multithreading_inc
     ${CMAKE_SOURCE_DIR}/include/multithreading/e_darts_mutex.h
     )

add_library( e_darts_multithreading ${e_darts_multithreading_src} ${e_darts_multithreading_inc})

set_target_properties(e_darts_multithreading PROPERTIES LINKER_LANGUAGE C
                                                        PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/multithreading/e_darts_mutex.h")