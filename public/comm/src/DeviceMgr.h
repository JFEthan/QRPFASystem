
#ifndef DEVICEMGR_H
#define DEVICEMGR_H
#include "comm_sdk.h"
#include <map>
#include <core/common/IDevice.h>
#include <mutex>

namespace accu {
    using namespace core::common;
class DeviceMgr
{
 public:
    DeviceMgr();
    ~DeviceMgr();  
    static DeviceMgr * GetDeviceMgr();
public:
    bool Initialize(MsgCallback *callback,void* userId);
    bool UnInitialize();
    long AddDevice(DEVICMODE device_mode,DEVICETYPE device_type);
    bool RemoveDevice(long deviceid);
    int  ConnectDevice(long deviceid,const char *port_path, uint32_t baudrate = 8000);
    void DisconnectDevice(long deviceid);
    
    IDevice*GetDeviceById(long deviceid);
    int  portList(std::vector<std::string> &comPortList);
   
   // int OperatorDevice(long deviceid,unsigned int OperatorType,const char * data,unsigned short dataSize);
protected:
    bool checkCOMMs(IDevice* devicePtr);
    bool OnCommunicate(std::vector<std::string> &comPortList);
private:
    static DeviceMgr * m_DeviceMgr ;
    MsgCallback *m_callback = NULL;
   void* m_userId = 0;
    
    using lock_guard_t = std::lock_guard<std::mutex>;
    std::mutex m_mapDevices_mutex;
    std::map<long,IDevice*> m_mapDevices;



};
}//accu

#endif // DEVICEMGR_H