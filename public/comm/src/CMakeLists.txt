include_directories(..)
include_directories(.)

set(CMAKE_INCLUDE_CURRENT_DIR ON)




include(Files.cmake)
include(../core/serial/Files.cmake)
include(../core/serial/impl/windows/Files.cmake)
include(../core/network/Files.cmake)
include(../core/common/Files.cmake)
include(../core/base/Files.cmake)
include(../core/FIR-Matlab/Files.cmake)

ADD_LIBRARY (accu_comm_sdk SHARED  ${PROJECT_MOC_SRCS} ${COMM_DIR_SRCS} ${COMM_MOC_HDRS} ${SERIAL_MOC_HDRS} 
${SERIAL_DIR_SRCS}  ${WINDOWS_DIR_SRCS} ${WINDOWS_MOC_HDRS} ${NETWORK_DIR_SRCS} ${NETWORK_MOC_HDRS} 
${COMMON_DIR_SRCS} ${COMMON_MOC_HDRS} ${BASE_DIR_SRCS} ${BASE_MOC_HDRS} ${FIR_Matlab_MOC_HDRS} ${FIR_Matlab_DIR_SRCS})

# TARGET_LINK_LIBRARIES(accu_comm_sdk core)
#  target_link_libraries(comm_sdk PRIVATE Qt5::Core Qt5::Widgets Qt5::Quick)