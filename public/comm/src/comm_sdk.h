
#ifndef COMM_SDK_H_
#define COMM_SDK_H_

#include <string>
#include <vector>

#ifdef BUILD_ACCU_COMM_SDK_DLL 
#define ACCU_COMM_SDK_DLL __declspec(dllexport) 
#else 
#define ACCU_COMM_SDK_DLL __declspec(dllimport) 
#endif

typedef void __stdcall MsgCallback(long  deviceHander,int msgType, void* data, unsigned int dataSize, void* userId);

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
* Return SDK's version information in a numeric form.
* @param version Pointer to a version for returning the version information.
*/
ACCU_COMM_SDK_DLL void COMM_GetSdkVersion(char *version);

/**
 * Initialize the SDK.
 * @return true if successfully initialized, otherwise false.
 */
ACCU_COMM_SDK_DLL bool COMM_Initialize(MsgCallback *callback,void* userId);

/**
 * UnInitialize the SDK.
 * @return true if successfully UnInitialized, otherwise false.
 */
ACCU_COMM_SDK_DLL bool COMM_UnInitialize();

/**
 * add the  deviceid for device.
 * @return deviceid if successfully UnInitialized, otherwise -1.
 */
ACCU_COMM_SDK_DLL long COMM_AddDeviceHnadler(int deviceMode,int devicetype );

/**
 * remove the  handler for device.
 * @return handler if successfully UnInitialized, otherwise -1.
 */
ACCU_COMM_SDK_DLL bool COMM_RemoveDeviceHnadler(long deviceid);

/**
 * port_path if devicetype is 1 the port_path is com post example COM1 or devicetype is.
 * @return handler if successfully UnInitialized, otherwise -1.
 */
ACCU_COMM_SDK_DLL int COMM_ConnectDevice(long deviceid,const char *port_path, uint32_t baudrate = 8000);

ACCU_COMM_SDK_DLL void COMM_DisconnectDevice(long deviceid);


// /**
//  * Operator the device cmd ADAS1000
//  * @return 
//  */
// ACCU_COMM_SDK_DLL int OperatorDevice(long deviceid,unsigned int OperatorType,const char * data,unsigned short dataSize);

/**
   * @brief Turn on ECG \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
 ACCU_COMM_SDK_DLL int COMM_startECG(long deviceid) ;

  /**
   * @brief turn off ECG \n
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
   ACCU_COMM_SDK_DLL int  COMM_stopECG(long deviceid) ;


    /**
   * @brief Turn on IMP \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
   ACCU_COMM_SDK_DLL int  COMM_resetIMP(long deviceid) ;

     /**
   * @brief Turn on IMP \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
   ACCU_COMM_SDK_DLL int  COMM_startIMP(long deviceid) ;
 /**
   * @brief turn off IMP \n
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
   ACCU_COMM_SDK_DLL int  COMM_stopIMP(long deviceid) ;
 
  // /**
  //  * @brief set RWave Preduction Lead  \n
  //  * @return result status 0：不启用 1：导联Ⅰ 2：导联Ⅱ 3：导联Ⅲ
  //  * @retval RESULT_OK       success
  //  * @retval RESULT_FAILE    failed  RWAVE_PREDUCTION_LEAD
  //  */
  //  ACCU_COMM_SDK_DLL int  COMM_SetRWavePreductionLead(long deviceid,char status);

  //  /**
  //  * @brief set Configure Security Delay  \n
  //  * @return result tm   ms
  //  * @retval RESULT_OK       success
  //  * @retval RESULT_FAILE    failed  RWAVE_PREDUCTION_LEAD
  //  */
  //  ACCU_COMM_SDK_DLL int  COMM_SetConfigureSecurityDelay(long deviceid,uint32_t tm) ;

     /**
   * @brief set Configure Security Delay  \n
   * @return result tm   ms
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed  RWAVE_PREDUCTION_LEAD
   */
   ACCU_COMM_SDK_DLL int  COMM_SendTest(long deviceid,uint8_t cmd,uint8_t *test,int len) ;

   ACCU_COMM_SDK_DLL int  COMM_comPortList(std::vector<std::string> &comPortList) ;

   ACCU_COMM_SDK_DLL bool COMM_Setdeviceopt(long deviceid,int optcount, const void *optval, int optlen);

   ACCU_COMM_SDK_DLL bool COMM_Setdeviceoptt_RWaveLead(long deviceid,int lead);

   ACCU_COMM_SDK_DLL bool COMM_Setdeviceoptt_Delaytime(long deviceid,int delaytime);

   ACCU_COMM_SDK_DLL bool COMM_Setdeviceoptt_RRInterval(long deviceid,float RRInterval);
#ifdef __cplusplus
}
#endif

#endif  // COMM_SDK_H_
