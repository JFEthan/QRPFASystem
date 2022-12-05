SET(FIR_Matlab_DIR ${PROJECT_SOURCE_DIR}/core/FIR-Matlab)

SET(FIR_Matlab_DIR_SRCS
${FIR_Matlab_DIR}/windowsKaiser293.c 
${FIR_Matlab_DIR}/windowsKaiser992.c 
${FIR_Matlab_DIR}/windowsKaiser1259.c 
${FIR_Matlab_DIR}/windowsKaiser.c 
${FIR_Matlab_DIR}/windowsKaiser285.c 
${FIR_Matlab_DIR}/windowsKaiser420.c 
${FIR_Matlab_DIR}/notch005.c
)

SET(FIR_Matlab_MOC_HDRS
${FIR_Matlab_DIR}/rtwtypes.h 
${FIR_Matlab_DIR}/tmwtypes.h 
${FIR_Matlab_DIR}/windowsKaiser.h 
${FIR_Matlab_DIR}/windowsKaiser293.h 

)

