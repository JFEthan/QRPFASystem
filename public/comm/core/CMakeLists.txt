cmake_minimum_required(VERSION 3.20)
project (core)

# add_subdirectory(base)
# add_subdirectory(common)
# add_subdirectory(network)
# add_subdirectory(serial)

include(base/Files.cmake)
include(common/Files.cmake)
include(network/Files.cmake)
include(serial/Files.cmake)
include(FIR-Matlab/Files.cmake)

# target_include_directories( ${PROJECT_NAME}
#     PUBLIC ${PROJECT_SOURCE_DIR}
