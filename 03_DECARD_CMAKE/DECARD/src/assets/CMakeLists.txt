# set( decard_assets_src
#      codeletsQueue.c
#      syncSlot.c
#      tpClosuresQueue.c)
# set( decard_assets_inc
#      ${CMAKE_SOURCE_DIR}/eDARTS/include/assets/codeletsQueue.h
#      ${CMAKE_SOURCE_DIR}/eDARTS/include/assets/syncSlot.h
#      ${CMAKE_SOURCE_DIR}/eDARTS/include/assets/tpClosuresQueue.h)

set( decard_assets_src 
     )

set( decard_assets_inc 
     ${CMAKE_SOURCE_DIR}/include/assets/Atomics.h
     ${CMAKE_SOURCE_DIR}/include/assets/AutoLock.h
     ${CMAKE_SOURCE_DIR}/include/assets/Lock.h
     ${CMAKE_SOURCE_DIR}/include/assets/node.h
     ${CMAKE_SOURCE_DIR}/include/assets/opr_code.h
     ${CMAKE_SOURCE_DIR}/include/assets/thread_safe_deque.h
     )
add_library( decard_assets SHARED ${decard_assets_src} ${decard_assets_inc})

set_target_properties(decard_assets PROPERTIES LINKER_LANGUAGE CXX
                                               PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/assets/Atomics.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/AutoLock.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/Lock.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/node.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/opr_code.h
                                                              ${CMAKE_SOURCE_DIR}/include/assets/thread_safe_deque.h"
                                                              )
