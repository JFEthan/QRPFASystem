SET(UNIX_DIR ${PROJECT_SOURCE_DIR}/core/serial/impl/unix)

SET(UNIX_DIR_SRCS
${SERIAL_DIR}/list_ports_linux.cpp 
${SERIAL_DIR}/unix_serial.cpp 
${SERIAL_DIR}/lock.c 

)

SET(UNIX_MOC_HDRS
${SERIAL_DIR}/unix.h 
${SERIAL_DIR}/lock.h 
${SERIAL_DIR}/unix_serial.h 

)

