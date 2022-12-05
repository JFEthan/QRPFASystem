

#include <sstream>
#include "CommConfig.h"
#include "comm_sdk.h"
#include "DeviceMgr.h"
#include <core/common/IDevice.h>
#include <core/common/logInf.h>

using namespace accu::core::common;


void COMM_GetSdkVersion(char *version)
{
  strcpy(version, ACCU_COMM_SDK_VERSION_STR);
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_GetSdkVersion");
}

bool COMM_Initialize(MsgCallback *callback, void* userId)
{
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_Initialize, callback:%x ,userId:%x",callback,userId);
 return accu::DeviceMgr::GetDeviceMgr()->Initialize(callback,userId);
 
}
bool COMM_UnInitialize()
{
   logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_UnInitialize");
	return accu::DeviceMgr::GetDeviceMgr()->UnInitialize();
}

long COMM_AddDeviceHnadler(int deviceMode,int devicetype)
{
   logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_AddDeviceHnadler,deviceMode:%d devicetype:%d",deviceMode,devicetype);
  return accu::DeviceMgr::GetDeviceMgr()->AddDevice((DEVICMODE)deviceMode,(DEVICETYPE)devicetype);
}

bool COMM_RemoveDeviceHnadler(long deviceid)
{
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_RemoveDeviceHnadler,deviceid:%d",deviceid);
  return accu::DeviceMgr::GetDeviceMgr()->RemoveDevice(deviceid);
}
int COMM_ConnectDevice(long deviceid,const char *port_path, uint32_t baudrate)
{
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_ConnectDevice,deviceid:%d port_path:%s baudrate:%d",deviceid,port_path,baudrate);
 return accu::DeviceMgr::GetDeviceMgr()->ConnectDevice(deviceid,port_path,baudrate);
}
void COMM_DisconnectDevice(long deviceid)
{
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_DisconnectDevice,deviceid:%d ",deviceid);
  return accu::DeviceMgr::GetDeviceMgr()->DisconnectDevice(deviceid);
}

// int OperatorDevice(long deviceid,unsigned int OperatorType,const char * data,unsigned short dataSize)
// {
//   return accu::DeviceMgr::GetDeviceMgr()->OperatorDevice(deviceid,OperatorType,data,dataSize);
// }


/**
   * @brief Turn on ECG \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
  int COMM_startECG(long deviceid) 
  {
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_startECG,deviceid:%d ",deviceid);
    accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->startECG();
    }
     return -1;
  }

  /**
   * @brief turn off ECG \n
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
  int COMM_stopECG(long deviceid) 
  {
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_stopECG,deviceid:%d ",deviceid);
      accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->stopECG();
    }
     return -1;
    
  }

int COMM_resetIMP(long deviceid) 
  {
     logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_resetIMP,deviceid:%d ",deviceid);
    accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->resetIMP();
    }
     return -1;
  }

     /**
   * @brief Turn on IMP \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
  int COMM_startIMP(long deviceid) 
  {
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_startIMP,deviceid:%d ",deviceid);
      accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->startIMP();
    }
     return -1;
  }
 /**
   * @brief turn off IMP \n
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
  int COMM_stopIMP(long deviceid) 
  {
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_stopIMP,deviceid:%d ",deviceid);
    accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->stopIMP();
    }
     return -1;

  }
 
  // /**
  //  * @brief set RWave Preduction Lead  \n
  //  * @return result status 0：不启用 1：导联Ⅰ 2：导联Ⅱ 3：导联Ⅲ
  //  * @retval RESULT_OK       success
  //  * @retval RESULT_FAILE    failed  RWAVE_PREDUCTION_LEAD
  //  */
  // int COMM_SetRWavePreductionLead(long deviceid,char status)
  // {
  //     accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
  //   if(ptr_device)
  //   {
  //     return ptr_device->SetRWavePreductionLead(status);
  //   }
  //    return -1;

  // }

  //  /**
  //  * @brief set Configure Security Delay  \n
  //  * @return result tm   ms
  //  * @retval RESULT_OK       success
  //  * @retval RESULT_FAILE    failed  RWAVE_PREDUCTION_LEAD
  //  */
  // int COMM_SetConfigureSecurityDelay(long deviceid,uint32_t tm) 
  // {
  //     accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
  //   if(ptr_device)
  //   {
  //     return ptr_device->SetConfigureSecurityDelay(tm);
  //   }
  //    return -1;
  // }

 
  int  COMM_SendTest(long deviceid,uint8_t cmd,uint8_t *test,int len) 
  {
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_SendTest,deviceid:%d cmd:%d",deviceid,cmd);
    accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->SendTest(cmd,test,len);
    }
     return -1;
  }
  int  COMM_comPortList(std::vector<std::string> &comPortList) 
  {
    return accu::DeviceMgr::GetDeviceMgr()->portList(comPortList);
  }

  bool COMM_Setdeviceopt(long deviceid,int optcount, const void *optval, int optlen)
  {
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_Setdeviceopt,deviceid:%d optcount:%d optval:%s cmd:%d optlen:%d",deviceid,optcount,(char*)optval,optlen);
    accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->setdeviceopt(optcount,optval,optlen);
    }
    return false;
  }

  bool COMM_Setdeviceoptt_RWaveLead(long deviceid,int lead)
  {
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_Setdeviceoptt_RWaveLead,deviceid:%d lead:%d ",deviceid,lead);
  accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->setdeviceopt_RWaveLead(lead);
    }
    return false;
  }
  bool COMM_Setdeviceoptt_Delaytime(long deviceid,int delaytime)
  {
     logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_Setdeviceoptt_Delaytime,deviceid:%d delaytime:%d ",deviceid,delaytime);
    accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->setdeviceopt_Delaytime(delaytime);
    }
    return false;
  }
  bool COMM_Setdeviceoptt_RRInterval(long deviceid,float RRInterval)
  {
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"COMM_Setdeviceoptt_RRInterval,deviceid:%d RRInterval:%d ",deviceid,RRInterval);
    accu::core::common::IDevice * ptr_device = accu::DeviceMgr::GetDeviceMgr()->GetDeviceById(deviceid);
    if(ptr_device)
    {
      return ptr_device->setdeviceopt_RRInterval(RRInterval);
    }
    return false;
  }

  