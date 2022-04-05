include_directories(${MPI_INCLUDE_PATH})
add_executable(VectorAddition VectorAddition.cpp)
target_link_libraries(VectorAddition PRIVATE ${MPI_LIBRARIES}
                                 PRIVATE decard)