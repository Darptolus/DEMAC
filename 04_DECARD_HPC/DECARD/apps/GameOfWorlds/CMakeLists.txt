add_executable(GameOfWorlds GameOfWorlds.cpp)
target_link_libraries(GameOfWorlds PRIVATE decard
                                   PRIVATE ${MPI_LIBRARIES})