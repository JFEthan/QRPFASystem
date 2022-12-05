
cmake_minimum_required(VERSION 3.20)
project (network)

include(Files.cmake)

IF (WIN32)
add_to_accu_libraries(setupapi ws2_32)
ELSE()
add_to_accu_libraries(rt)
ENDIF()                                            
