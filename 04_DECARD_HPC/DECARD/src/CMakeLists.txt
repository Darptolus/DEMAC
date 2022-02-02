add_subdirectory( assets )
add_subdirectory( codeletModel )
add_subdirectory( runtime )

set( decard_src
     runtime/Runtime.cpp
     )

set( decard_inc
     ${CMAKE_SOURCE_DIR}/include/runtime/Runtime.hpp
     ${CMAKE_SOURCE_DIR}/include/runtime/Messages.hpp
     )

add_library( decard SHARED ${decard_src} ${decard_inc} )

target_link_libraries(decard decard_assets 
                             decard_codeletModel 
                             decard_runtime_host 
                             decard_runtime_device
                             )

set_target_properties(decard PROPERTIES LINKER_LANGUAGE CXX
                                        PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/decard.hpp
                                                       ${CMAKE_SOURCE_DIR}/include/Messages.hpp")
