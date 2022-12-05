/*
 * File: windowsKaiser.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 18-Mar-2022 15:52:45
 */

#ifndef WINDOWSKAISER_H
#define WINDOWSKAISER_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
//#include "windowsKaiser_types.h"

#ifdef __cplusplus
extern "C"{
#endif

//#define FILTER_COUNTS_293   600
//#define FILTER_WIN_293      100
//#define FILTER_GRPLEN_293   293
//#define FILTER_PHASE_DELAY_293  1.83469   //相延迟*频率=弧度

//#define FILTER_COUNTS_992   2000
//#define FILTER_WIN_992      1000
//#define FILTER_GRPLEN_992   992
//#define FILTER_PHASE_DELAY  6.226637

#define FILTER_COUNTS_F  500
#define FILTER_WIN_F     100
#define FILTER_GRPLEN_F    285

extern void  notchInit();
extern float notchProc(float signal);

extern void Fir420Init();
extern void bandFilterBFunc260(const double b_signal[500], double filterSignal[500]);

extern void bandFilterBFunc543(const double b_signal[1000], double filterSignal[1000]);

//extern void windowsKaiser293(double b_signal[600], double filterSignal[600]);

extern void windowsKaiser285(const double b_signal[500], double filterSignal[500]);

extern float Fir420FilterExt(float data);



/* Function Declarations */
//extern void windowsKaiser992(const double b_signal[2000], double filterSignal[2000]);

//extern void windowsKaiser600(const double b_signal[2000], double filterSignal[2000]);

//extern void windowsKaiser1259(const double b_signal[4000], double filterSignal[4000]);


#ifdef __cplusplus
}
#endif

#endif

/*
 * File trailer for windowsKaiser.h
 *
 * [EOF]
 */
