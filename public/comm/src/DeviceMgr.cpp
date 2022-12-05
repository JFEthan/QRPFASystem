#include "DeviceMgr.h"
#include "RFDevice.h"

#include <core/common/logInf.h>

namespace accu {
DeviceMgr *DeviceMgr::m_DeviceMgr = nullptr;

DeviceMgr::DeviceMgr() {}
DeviceMgr::~DeviceMgr() {}
DeviceMgr *DeviceMgr::GetDeviceMgr() {
  if (m_DeviceMgr == nullptr) {
    m_DeviceMgr = new DeviceMgr();
  }
  return m_DeviceMgr;
}
bool DeviceMgr::Initialize(MsgCallback *callback, void *userId) {
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DeviceMgr::Initialize, callback:%x ,userId:%x",callback,userId);
  m_callback = callback;
  m_userId = userId;
  return true;
}
bool DeviceMgr::UnInitialize() { 
   logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DeviceMgr::UnInitialize");
  return true; 
  }

long DeviceMgr::AddDevice(DEVICMODE device_mode, DEVICETYPE device_type) {
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DeviceMgr::AddDevice");
  long deviceid = -1;
  IDevice *deive_ptr = nullptr;
  switch (device_type) {
  case DEVICE_TYPE_RF: {
    deive_ptr = new RFDevice(device_mode);
    deive_ptr->Initialize(m_callback, m_userId);
  } break;
  }
  if (deive_ptr != nullptr) {
    deviceid = deive_ptr->getDeviceId();
    {
      lock_guard_t _locker(m_mapDevices_mutex);
       m_mapDevices.insert(std::pair<long, IDevice *>(deviceid, deive_ptr));
    }
    
  }

  return deviceid;
}
bool DeviceMgr::RemoveDevice(long deviceid) {
  
  lock_guard_t _locker(m_mapDevices_mutex);
  std::map<long, IDevice *>::iterator it = m_mapDevices.find(deviceid);
  if (it != m_mapDevices.end()) {
    IDevice *deive_ptr = it->second;
    if (deive_ptr != nullptr) {
      delete deive_ptr;
	  deive_ptr = NULL;
    }
    m_mapDevices.erase(it);
  }

  return true;
}
int DeviceMgr::ConnectDevice(long deviceid, const char *port_path, uint32_t baudrate) {
  lock_guard_t _locker(m_mapDevices_mutex);
  std::map<long, IDevice *>::iterator it = m_mapDevices.find(deviceid);
  if (it != m_mapDevices.end()) {
    IDevice *deive_ptr = it->second;
    if (deive_ptr != nullptr) {
      std::string _SerialPort = port_path;
      // Is it COMX, X>4? ->  "\\.\COMX"
      if (_SerialPort.size() >= 3) {
        if (tolower(_SerialPort[0]) == 'c' && tolower(_SerialPort[1]) == 'o' &&
            tolower(_SerialPort[2]) == 'm') {
          // Need to add "\\.\"?
          if (_SerialPort.size() > 4 || _SerialPort[3] > '4') {
            _SerialPort = std::string("\\\\.\\") + _SerialPort;
          }
        }
      }
      return deive_ptr->connectDevice(_SerialPort.c_str(), baudrate);
    }
  }
  return -1;
}
void DeviceMgr::DisconnectDevice(long deviceid) {
  lock_guard_t _locker(m_mapDevices_mutex);
  std::map<long, IDevice *>::iterator it = m_mapDevices.find(deviceid);
  if (it != m_mapDevices.end()) {
    IDevice *deive_ptr = it->second;
    if (deive_ptr != nullptr) {
      return deive_ptr->disconnect();
    }
  }
}

// int DeviceMgr::OperatorDevice(long deviceid,unsigned int OperatorType,const char * data,unsigned
// short dataSize)
// {

//     // std::map<long,IDevice*>::iterator it = m_mapDevices.find(deviceid);
//     // if(it != m_mapDevices.end())
//     // {
//     //    IDevice* deive_ptr = it->second;
//     //     if(deive_ptr != nullptr)
//     //     {
//     //        return deive_ptr->OperatorDevice(OperatorType,data,dataSize);
//     //     }
//     // }
//    return -1;
// }
IDevice *DeviceMgr::GetDeviceById(long deviceid) {
  lock_guard_t _locker(m_mapDevices_mutex);
  IDevice *device_ptr = NULL;
  std::map<long, IDevice *>::iterator it = m_mapDevices.find(deviceid);
  if (it != m_mapDevices.end()) {
    device_ptr = it->second;
  }
  return device_ptr;
}
int DeviceMgr::portList(std::vector<std::string> &comPortList) {
  //     using namespace core;
  // using namespace core::common;
  int nRes = 0;
  std::vector<core::serial::PortInfo> lst = core::serial::list_ports();
  std::map<std::string, std::string> ports;

  for (std::vector<core::serial::PortInfo>::iterator it = lst.begin(); it != lst.end(); it++) {
    std::string port = (*it).port;
	std::string hardware_id = (*it).hardware_id;
	int npos = hardware_id.find("VID_0403&PID_6001");
	if (npos  > 0)
	{
		comPortList.push_back(port);
		nRes++;
	}
    
  }

  return nRes;
}

 

} // namespace accu
