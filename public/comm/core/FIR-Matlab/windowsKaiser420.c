/*
 * File: windowsKaiser.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 25-Jul-2022 09:22:48
 */

/* Include Files */
#include <string.h>
#include "windowsKaiser.h"

/* Function Definitions */
#define FIR_WIN_K_ORDER421 421
static const double dv0[421] = { -0.00030775715919275016,
  0.00018953384212337868, 0.00097067701082209894, 0.0017946588027314234,
  0.0024048636223732611, 0.0026074443488416834, 0.0023312492649320447,
  0.0016507940689723685, 0.000764890627819485, -6.296490780390227E-5,
  -0.00058458518373425935, -0.00064319584655538943, -0.00022232936469580007,
  0.00054772241169109826, 0.0014282980749407415, 0.0021443069892731629,
  0.002468050660465646, 0.0022894005543490268, 0.0016507203140804592,
  0.00073548335358608011, -0.00018641177769467321, -0.00084034729272726178,
  -0.0010309955803159555, -0.00070295008686330348, 4.08229816591604E-5,
  0.00096880763534310335, 0.0017904264487299043, 0.0022443566411159293,
  0.0021788509208383305, 0.00159939561630161, 0.00066876371099885729,
  -0.00034107712040539438, -0.0011310156079588627, -0.0014660728210120193,
  -0.0012480702511616468, -0.00054723953916281968, 0.00041736541644620142,
  0.0013432409100776934, 0.0019361046827963899, 0.0020001087635008255,
  0.0014989630331811803, 0.00056892071362604952, -0.00052091279671090417,
  -0.0014494178772350129, -0.0019411863194428523, -0.0018514189385191982,
  -0.0012118509271301008, -0.00022317510438902151, 0.0008043664154463745,
  0.0015446663606502836, 0.0017554501883026094, 0.0013535370888144136,
  0.00044231672559289582, -0.00071756920961274929, -0.0017860784646710572,
  -0.0024469291765232344, -0.00250480392888313, -0.001946754741142714,
  -0.00094854281668737661, 0.00017674413724310192, 0.0010728166371562231,
  0.0014487873373658893, 0.0011691852256958085, 0.00029756373216146414,
  -0.00092027668591579815, -0.0021291295111308044, -0.0029717395298311374,
  -0.0031982663240088445, -0.00274435348886879, -0.0017534533920421278,
  -0.00053578866030960239, 0.00052437121907993565, 0.0010854467642341433,
  0.00095387972695295045, 0.00014562527641207997, -0.0011156698555687254,
  -0.0024641699539739237, -0.0035019015825437454, -0.0039202947037785462,
  -0.003596144498796367, -0.0026322050050903338, -0.0013291363962196492,
  -9.63536168790061E-5, 0.00067184431424625933, 0.00071744246568301786,
  2.8502322414490437E-18, -0.0012874792795629262, -0.0027739986619813137,
  -0.0040214886385338674, -0.0046580931982633184, -0.0044933323540840156,
  -0.003579543933643355, -0.002199882625580617, -0.00078533078045680009,
  0.00021503177864443283, 0.00047152079633645821, -0.0001229155421851553,
  -0.00141595736863776, -0.003038092106977188, -0.0045121638711675474,
  -0.0053978959509008925, -0.0054276962503896371, -0.004591945420976894,
  -0.0031466957495488224, -0.0015399965079484796, -0.000277933231172499,
  0.0002296619810707969, -0.00020310734754728189, -0.0014767949727973286,
  -0.0032315885191002828, -0.0049526851538750638, -0.00612531877705296,
  -0.006392866193619679, -0.0056695923664769081, -0.0041725314557086,
  -0.00236107401634739, -0.00080089382249669511, 7.6114269304984049E-6,
  -0.00021552677754908559, -0.0014390560973683865, -0.0033233148117690352,
  -0.0053178158373701871, -0.0068257374481083318, -0.00738632568154865,
  -0.0068196317273230391, -0.0052883086990292236, -0.00325559793117542,
  -0.0013499905861878447, -0.00017591436706226179, -0.00012727358949384549,
  -0.0012611480791012675, -0.0032718862019390331, -0.0055760176820642731,
  -0.0074846810052980979, -0.0084128376500055783, -0.0080619804283186946,
  -0.0065195525233185461, -0.0042423670272257888, -0.0019260356512573367,
  -0.00029743049393966077, 0.00010819297632550931, -0.00088260446600566932,
  -0.0030176668153430695, -0.0056844971749673012, -0.0080882278684597155,
  -0.009490948990162713, -0.0094407167326440479, -0.0079195984898692562,
  -0.0053628108541774249, -0.0025390696502499512, -0.00032452234970205214,
  0.00056371545180038661, -0.00020610269045447986, -0.0024649830020992448,
  -0.0055779499650987626, -0.00862339227850198, -0.010666946325279857,
  -0.01104915404026181, -0.009599057337718259, -0.0067054279736134138,
  -0.0032184499114686271, -0.00020559610897182804, 0.0013690011521824369,
  0.00094628266627269479, -0.0014382755510419151, -0.0051402243352658486,
  -0.009078488809060278, -0.012049610321588732, -0.013093685503773603,
  -0.011802081766882772, -0.0084699275161742738, -0.00403940066801157,
  0.00015878012903767667, 0.00279631997923252, 0.0029576698911890067,
  0.00044510989096038193, -0.0041202578525445285, -0.0094434632985207455,
  -0.013915855275921858, -0.016092741884014421, -0.015149602858897349,
  -0.011176538635659474, -0.0052113472404309814, 0.0010153851849553162,
  0.0055645503033840242, 0.0068707190746106865, 0.0042528545378165191,
  -0.0018058241306678239, -0.0097101795689947135, -0.017147277952108572,
  -0.021731163745709672, -0.021726322848209786, -0.016641612629961951,
  -0.0075080213698625435, 0.003269508186255527, 0.012482241333837827,
  0.016985201197638272, 0.014643432214956641, 0.0050986903436671446,
  -0.0098726526803918826, -0.026545945587139484, -0.039977782100087407,
  -0.045133019984816981, -0.038149196154613978, -0.017425948101068015,
  0.01574656273512182, 0.057204859605407866, 0.1006068406175465,
  0.13866933936004572, 0.16467609150317455, 0.17391020788244457,
  0.16467609150317455, 0.13866933936004572, 0.1006068406175465,
  0.057204859605407866, 0.01574656273512182, -0.017425948101068015,
  -0.038149196154613978, -0.045133019984816981, -0.039977782100087407,
  -0.026545945587139484, -0.0098726526803918826, 0.0050986903436671446,
  0.014643432214956641, 0.016985201197638272, 0.012482241333837827,
  0.003269508186255527, -0.0075080213698625435, -0.016641612629961951,
  -0.021726322848209786, -0.021731163745709672, -0.017147277952108572,
  -0.0097101795689947135, -0.0018058241306678239, 0.0042528545378165191,
  0.0068707190746106865, 0.0055645503033840242, 0.0010153851849553162,
  -0.0052113472404309814, -0.011176538635659474, -0.015149602858897349,
  -0.016092741884014421, -0.013915855275921858, -0.0094434632985207455,
  -0.0041202578525445285, 0.00044510989096038193, 0.0029576698911890067,
  0.00279631997923252, 0.00015878012903767667, -0.00403940066801157,
  -0.0084699275161742738, -0.011802081766882772, -0.013093685503773603,
  -0.012049610321588732, -0.009078488809060278, -0.0051402243352658486,
  -0.0014382755510419151, 0.00094628266627269479, 0.0013690011521824369,
  -0.00020559610897182804, -0.0032184499114686271, -0.0067054279736134138,
  -0.009599057337718259, -0.01104915404026181, -0.010666946325279857,
  -0.00862339227850198, -0.0055779499650987626, -0.0024649830020992448,
  -0.00020610269045447986, 0.00056371545180038661, -0.00032452234970205214,
  -0.0025390696502499512, -0.0053628108541774249, -0.0079195984898692562,
  -0.0094407167326440479, -0.009490948990162713, -0.0080882278684597155,
  -0.0056844971749673012, -0.0030176668153430695, -0.00088260446600566932,
  0.00010819297632550931, -0.00029743049393966077, -0.0019260356512573367,
  -0.0042423670272257888, -0.0065195525233185461, -0.0080619804283186946,
  -0.0084128376500055783, -0.0074846810052980979, -0.0055760176820642731,
  -0.0032718862019390331, -0.0012611480791012675, -0.00012727358949384549,
  -0.00017591436706226179, -0.0013499905861878447, -0.00325559793117542,
  -0.0052883086990292236, -0.0068196317273230391, -0.00738632568154865,
  -0.0068257374481083318, -0.0053178158373701871, -0.0033233148117690352,
  -0.0014390560973683865, -0.00021552677754908559, 7.6114269304984049E-6,
  -0.00080089382249669511, -0.00236107401634739, -0.0041725314557086,
  -0.0056695923664769081, -0.006392866193619679, -0.00612531877705296,
  -0.0049526851538750638, -0.0032315885191002828, -0.0014767949727973286,
  -0.00020310734754728189, 0.0002296619810707969, -0.000277933231172499,
  -0.0015399965079484796, -0.0031466957495488224, -0.004591945420976894,
  -0.0054276962503896371, -0.0053978959509008925, -0.0045121638711675474,
  -0.003038092106977188, -0.00141595736863776, -0.0001229155421851553,
  0.00047152079633645821, 0.00021503177864443283, -0.00078533078045680009,
  -0.002199882625580617, -0.003579543933643355, -0.0044933323540840156,
  -0.0046580931982633184, -0.0040214886385338674, -0.0027739986619813137,
  -0.0012874792795629262, 2.8502322414490437E-18, 0.00071744246568301786,
  0.00067184431424625933, -9.63536168790061E-5, -0.0013291363962196492,
  -0.0026322050050903338, -0.003596144498796367, -0.0039202947037785462,
  -0.0035019015825437454, -0.0024641699539739237, -0.0011156698555687254,
  0.00014562527641207997, 0.00095387972695295045, 0.0010854467642341433,
  0.00052437121907993565, -0.00053578866030960239, -0.0017534533920421278,
  -0.00274435348886879, -0.0031982663240088445, -0.0029717395298311374,
  -0.0021291295111308044, -0.00092027668591579815, 0.00029756373216146414,
  0.0011691852256958085, 0.0014487873373658893, 0.0010728166371562231,
  0.00017674413724310192, -0.00094854281668737661, -0.001946754741142714,
  -0.00250480392888313, -0.0024469291765232344, -0.0017860784646710572,
  -0.00071756920961274929, 0.00044231672559289582, 0.0013535370888144136,
  0.0017554501883026094, 0.0015446663606502836, 0.0008043664154463745,
  -0.00022317510438902151, -0.0012118509271301008, -0.0018514189385191982,
  -0.0019411863194428523, -0.0014494178772350129, -0.00052091279671090417,
  0.00056892071362604952, 0.0014989630331811803, 0.0020001087635008255,
  0.0019361046827963899, 0.0013432409100776934, 0.00041736541644620142,
  -0.00054723953916281968, -0.0012480702511616468, -0.0014660728210120193,
  -0.0011310156079588627, -0.00034107712040539438, 0.00066876371099885729,
  0.00159939561630161, 0.0021788509208383305, 0.0022443566411159293,
  0.0017904264487299043, 0.00096880763534310335, 4.08229816591604E-5,
  -0.00070295008686330348, -0.0010309955803159555, -0.00084034729272726178,
  -0.00018641177769467321, 0.00073548335358608011, 0.0016507203140804592,
  0.0022894005543490268, 0.002468050660465646, 0.0021443069892731629,
  0.0014282980749407415, 0.00054772241169109826, -0.00022232936469580007,
  -0.00064319584655538943, -0.00058458518373425935, -6.296490780390227E-5,
  0.000764890627819485, 0.0016507940689723685, 0.0023312492649320447,
  0.0026074443488416834, 0.0024048636223732611, 0.0017946588027314234,
  0.00097067701082209894, 0.00018953384212337868, -0.00030775715919275016 };


static int m_cnt = 0;
static float m_buff[FIR_WIN_K_ORDER421] = {0};

void Fir420Init()
{
    m_cnt = 0;
}

float Fir420FilterExt(float data)
{
    float filterSignal = 0.0;
    if (m_cnt < FIR_WIN_K_ORDER421)
    {
        m_cnt++;

    }
    else
    {
        for (int i=0;i<m_cnt-1;i++)
        {
            m_buff[i] = m_buff[i+1];
        }
    }
    m_buff[m_cnt-1] = data;
    for (int i=0;i<m_cnt;i++)
    {
        filterSignal += dv0[i]*m_buff[m_cnt-i-1];
    }
    return filterSignal;
}
/*
 * WINDOWSKAISER Returns a discrete-time filter object.
 * Arguments    : const float b_signal[600]
 *                float filterSignal[600]
 * Return Type  : void
 */
void windowsKaiser421(const float b_signal[600], float filterSignal[600])
{
  int k;
  int naxpy;
  int j;
  int filterSignal_tmp;
  /*  MATLAB Code */
  /*  Generated by MATLAB(R) 9.5 and Signal Processing Toolbox 8.1. */
  /*  Generated on: 17-Mar-2022 20:06:41 */
  /*  FIR Window Bandpass filter designed using the FIR1 function. */
  /*  All frequency values are in Hz. */
  /*  Sampling Frequency */
  /*  Fstop1 = 0.5;      % First Stopband Frequency */
  /*  Fpass1 = 1.0;        % First Passband Frequency */
  /*  Fpass2 = 22;       % Second Passband Frequency */
  /*  Fstop2 = 22.5;     % Second Stopband Frequency */
  /*  Dstop1 = 0.1;      % First Stopband Attenuation  0.3 */
  /*  Dpass  = 0.1;      % Passband Ripple            0.25  */
  /*  Dstop2 = 0.1;      % Second Stopband Attenuation 0.3 */
  /*  flag   = 'scale';  % Sampling Flag */
  /*  First Stopband Frequency */
  /*  First Passband Frequency */
  /*  Second Passband Frequency */
  /*  Second Stopband Frequency */
  /*  First Stopband Attenuation  0.2 */
  /*  Passband Ripple            0.15    */
  /*  Second Stopband Attenuation 0.2 */
  /*  Sampling Flag */
  /*  Calculate the order from the parameters using KAISERORD. 595 */
  /*  Calculate the coefficients using the FIR1 function. */
  memset(&filterSignal[0], 0, 600U * sizeof(float));
  for (k = 0; k < 179; k++) {
    for (j = 0; j < 421; j++) {
      filterSignal_tmp = k + j;
      filterSignal[filterSignal_tmp] += b_signal[k] * (float)dv0[j];
    }
  }

  naxpy = 420;
  for (k = 0; k < 421; k++) {
    for (j = 0; j <= naxpy; j++) {
      filterSignal_tmp = (k + j) + 179;
      filterSignal[filterSignal_tmp] += b_signal[k + 179] * (float)dv0[j];
    }

    naxpy--;
  }

  /* filterSignal = filtfilt(b,1,signal); */
  /*  [EOF] */
}

/*
 * File trailer for windowsKaiser.c
 *
 * [EOF]
 */