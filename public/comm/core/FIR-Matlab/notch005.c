
#include <string.h>
#include "windowsKaiser.h"

#define NOTCH_005_ORDER 3
static const double B[NOTCH_005_ORDER] = { 0.99874791227714965, -1.9974942473950204, 0.99874791227714965 };  //numerator

static const double A[NOTCH_005_ORDER] = { 1.0, -1.9974942473950204, 0.9974958245542993 }; //denominator

static double w_x[NOTCH_005_ORDER] = {0.0};
static double w_y[NOTCH_005_ORDER] = {0.0};
static double Gain = 1.0;

void notchInit()
{
	for (int i=0;i<NOTCH_005_ORDER;i++)
	{
        w_x[i] = 0.0;
        w_y[i] = 0.0;
	}
	
}

float notchProc(float signal)
{
	float v = 0.0;
    w_x[0]=signal;
    w_y[0]=(B[0]*w_x[0]+B[1]*w_x[1]+B[2]*w_x[2])*Gain-w_y[1]*A[1]-w_y[2]*A[2];
    v=w_y[0]/A[0];
    w_x[2]=w_x[1];
    w_x[1]=w_x[0];
    w_y[2]=w_y[1];
    w_y[1]=w_y[0];

	return v;
}

