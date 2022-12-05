SET(WINDOWS_DIR ${PROJECT_SOURCE_DIR}/core/serial/impl/windows)

SET(WINDOWS_DIR_SRCS
${WINDOWS_DIR}/list_ports_win.cpp 
${WINDOWS_DIR}/win_serial.cpp 
)

SET(WINDOWS_MOC_HDRS
${WINDOWS_DIR}/win_serial.h 
${WINDOWS_DIR}/win.h 
)

