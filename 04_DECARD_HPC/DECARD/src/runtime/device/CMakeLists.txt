set( decard_runtime_device_src
     ddarts.cpp
     )

set( decard_runtime_device_inc
     ${CMAKE_SOURCE_DIR}/include/runtime/device/ddarts.hpp
     )

add_library( decard_runtime_device ${decard_runtime_device_src} ${decard_runtime_device_inc})

set_target_properties(decard_runtime_device PROPERTIES LINKER_LANGUAGE CXX
                                                       PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/runtime/device/ddarts.hpp")

# install(TARGETS decard_runtime_device
#         EXPORT decardibraryDepends
#         ARCHIVE DESTINATION decard/lib/             COMPONENT bin
#         PUBLIC_HEADER DESTINATION decard/include/   COMPONENT dev)