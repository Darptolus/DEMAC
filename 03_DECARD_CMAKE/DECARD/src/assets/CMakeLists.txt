set( decard_assets_src 
     )

set( decard_assets_inc 
     ${CMAKE_SOURCE_DIR}/include/assets/Atomics.h
     ${CMAKE_SOURCE_DIR}/include/assets/AutoLock.h
     ${CMAKE_SOURCE_DIR}/include/assets/Lock.h
     ${CMAKE_SOURCE_DIR}/include/assets/thread_safe_deque.h
     ${CMAKE_SOURCE_DIR}/include/assets/DECARD_tools.hpp
     )
add_library( decard_assets SHARED ${decard_assets_src} ${decard_assets_inc})

set_target_properties(decard_assets PROPERTIES LINKER_LANGUAGE CXX
                                               PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/assets/Atomics.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/AutoLock.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/Lock.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/thread_safe_deque.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/DECARD_tools.hpp"
                                                              )
