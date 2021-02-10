add_subdirectory( assets )
add_subdirectory( codeletModel )
add_subdirectory( runtime )

set( decard_src
     runtime/runtime.cpp
     )

set( decard_inc
     ${CMAKE_SOURCE_DIR}/include/runtime/runtime.hpp
     )

add_library( decard SHARED ${decard_src} ${decard_inc} )

target_link_libraries(decard decard_assets 
                             decard_codeletModel 
                             decard_runtime_host 
                             decard_runtime_device
                             )
