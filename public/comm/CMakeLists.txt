cmake_minimum_required(VERSION 3.20)
project(accu_comm_sdk)



# the version number, using semantic versioning
set(ACCU_COMM_SDK_VERSION_MAJOR 1)
set(ACCU_COMM_SDK_VERSION_MINOR 0)
set(ACCU_COMM_SDK_VERSION_PATCH 0)
set(ACCU_COMM_SDK_VERSION ${ACCU_COMM_SDK_VERSION_MAJOR}.${ACCU_COMM_SDK_VERSION_MINOR}.${ACCU_COMM_SDK_VERSION_PATCH})

##################################################
#c++ 11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

##########################################################


##########################################################
# Detect wordsize:
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)  # Size in bytes!
	SET(CMAKE_MRPT_WORD_SIZE 64)
ELSE()
	SET(CMAKE_MRPT_WORD_SIZE 32)
ENDIF()



#####################################################
# add cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(SDK_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#############################################################################
# include cmake file
include(common/accu_base)
include(install_package)


#############################################################################
# Policy CMP0023 allows to mix old and new interfaces of target_link_libraries
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW) # We don't want to mix relative and absolute paths in linker lib lists.
	cmake_policy(SET CMP0005 NEW) # Escape definitions (-D) strings
    if(POLICY CMP0020)
        cmake_policy(SET CMP0020 NEW)
    endif()
	if(POLICY CMP0053)
		cmake_policy(SET CMP0053 OLD) # Simplify variable reference and escape sequence evaluation.
	endif()
	if(POLICY CMP0037)
		cmake_policy(SET CMP0037 OLD)  # Allow defining target "test"
	endif()
	if(POLICY CMP0043)
		cmake_policy(SET CMP0043 OLD) #  Ignore COMPILE_DEFINITIONS_<Config> properties.
	endif()
endif()

#############################################################################
# option
# option( BUILD_SHARED_LIBS "Build shared libraries." OFF)
# option( BUILD_EXAMPLES "Build Example." ON)
# option( BUILD_CSHARP "Build CSharp." OFF)
# option( BUILD_TEST "Build Test." ON)

############################################################################
# find package
# FIND_PACKAGE(SWIG)
# find_package(GTest)

# configure a header file to pass some of the CMAKE settings to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/CommConfig.h.in"
    "${PROJECT_BINARY_DIR}/CommConfig.h"
)




##################################################
message("ssssssssssssssssssss ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_BINARY_DIR}")
include_directories(
   ${PROJECT_SOURCE_DIR}/core/
   ${PROJECT_SOURCE_DIR}/src/
)
# lib directories
LINK_DIRECTORIES(
  ${PROJECT_BINARY_DIR}/lib
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/comm/lib)
##################################################


add_subdirectory(core)
add_subdirectory(src)
Add_Definitions(-DUNICODE -D_UNICODE -BUILD_ACCU_COMM_SDK_DLL)

