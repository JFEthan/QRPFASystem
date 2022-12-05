SET(COMM_DIR ${PROJECT_SOURCE_DIR}/src)

SET(COMM_DIR_SRCS
    ${COMM_DIR}/comm_sdk.cpp 
    ${COMM_DIR}/DeviceMgr.cpp 
    ${COMM_DIR}/RFDevice.cpp 
    ${COMM_DIR}/calcEcgData.cpp 
    
)

SET(COMM_MOC_HDRS
${COMM_DIR}/comm_sdk.h 
${COMM_DIR}/DeviceMgr.h 
${COMM_DIR}/RFDevice.h 
${COMM_DIR}/calcEcgData.h  
)

