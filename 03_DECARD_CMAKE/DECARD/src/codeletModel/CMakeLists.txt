set( decard_codeletModel_src 
     AbstractMachine.cpp
     Node.cpp
     ThreadedProcedure.cpp)

set( decard_codeletModel_inc 
     ${CMAKE_SOURCE_DIR}/include/codeletModel/AbstractMachine.hpp
     ${CMAKE_SOURCE_DIR}/include/codeletModel/ThreadedProcedure.hpp
     ${CMAKE_SOURCE_DIR}/include/codeletModel/NodeInterface.hpp
     ${CMAKE_SOURCE_DIR}/include/codeletModel/Node.hpp)

add_library( decard_codeletModel SHARED ${decard_codeletModel_src} ${decard_codeletModel_inc})

set_target_properties(decard_codeletModel PROPERTIES LINKER_LANGUAGE CXX
                                                     PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/codeletModel/AbstractMachine.hpp 
                                                                    ${CMAKE_SOURCE_DIR}/include/codeletModel/ThreadedProcedure.hpp
                                                                    ${CMAKE_SOURCE_DIR}/include/codeletModel/NodeInterface.hpp
                                                                    ${CMAKE_SOURCE_DIR}/include/codeletModel/Node.hpp")

# target_compile_options(decard_codeletModel PRIVATE -fopenmp)

# target_link_libraries(decard_codeletModel ${MPI_LIBRARIES})
