include_directories(${MPI_INCLUDE_PATH})
add_executable(HelloWorld HelloWorld.cpp)
target_link_libraries(HelloWorld PRIVATE ${MPI_LIBRARIES}
                                 PRIVATE decard)