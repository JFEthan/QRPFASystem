#include "RFDevice.h"
#include <core/base/thread.h>
#include <core/common/ChannelDevice.h>
#include <core/common/accu_protocol.h>
#include <core/common/comm_def.h>
#include <core/network/ActiveSocket.h>
#include <core/serial/common.h>
#include <core/serial/serial.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <core/common/logInf.h>


using namespace impl;

namespace accu {
using namespace core::serial;
using namespace core::common;
using namespace core::network;

RFDevice::RFDevice(DEVICMODE device_mode) : IDevice(device_mode) {
  m_DeviceType = DEVICE_TYPE_RF;
  _thread1= CLASS_THREAD(RFDevice, disposeECGData);
  _thread2 = CLASS_THREAD(RFDevice, disposeIMPData);
  _thread3 = CLASS_THREAD(RFDevice, disposeNomalData);

  is_ecg_dispose_thread = true;
  is_imp_dispose_thread = true;
  is_nomal_dispose_thread = true;
  hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"RFDevice::RFDevice");

}
RFDevice::~RFDevice() {
  ScopedLocker lck(_cmd_lock);
  disconnect();

  is_ecg_dispose_thread = false;
  is_imp_dispose_thread = false;
  is_nomal_dispose_thread = false;


  _thread1.terminate();
  _thread2.terminate();
  _thread3.terminate();

  {
    ScopedLocker l(_mapEcgData_lock);
    std::map<uint8_t,calcEcgData*>::iterator it = mapEcgData.begin(); 
    while(it != mapEcgData.end())
    {
      calcEcgData *calEcgData_ptr = it->second; it++;
      if(calEcgData_ptr)
      {
        delete calEcgData_ptr;
        calEcgData_ptr = NULL;
      }
    }
    mapEcgData.clear();
  }
  
  CloseHandle(hEvent);
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"RFDevice::~RFDevice");
}

result_t RFDevice::connectDevice(const char *port_path, uint32_t baudrate) {
  
  ScopedLocker lck(_cmd_lock);
  serial_port = port_path;
  m_baudrate = baudrate;

  if (!_serial) {
    if (m_deviceMode == DEVICE_MODE_TCP) {
      _serial = new CActiveSocket();
      logInf::getInstance()->LogString2(LogLevelEnum::INFO,"new CActiveSocket _serial:%x",_serial);
    } else {
      _serial = new serial::Serial(port_path, m_baudrate,
                                   serial::Timeout::simpleTimeout(DEFAULT_TIMEOUT));
     logInf::getInstance()->LogString2(LogLevelEnum::INFO,"new Serial _serial:%x",_serial);
    }

    _serial->bindport(port_path, baudrate);
  }

  {
    ScopedLocker l(_lock);

    if (!_serial->open()) {
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"Serial open failed _serial:%x",_serial);
      setDriverError(NotOpenError);
      return RESULT_FAIL;
    }
   if(m_isConnected == false) {
    logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"Serial connecect success _serial:%x",_serial);
     m_isConnected = true;
     createThread();
   }
  }
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"send cmd:%d",ACCU_CMD_CHIP_STATUS_GET);
   result_t ans = RESULT_FAIL;
  if ((ans = sendCommand(ACCU_CMD_CHIP_STATUS_GET)) != RESULT_OK) {
    logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"send cmd:%d failed :%d",ACCU_CMD_CHIP_STATUS_GET,ans);
     return ans;
    } 

    DWORD dw = WaitForSingleObject(hEvent,1000);
    switch (dw)
    {
    case WAIT_OBJECT_0:
     ans= RESULT_OK;
    break;

    case WAIT_TIMEOUT:
    // 等待时间超过1秒
		ans = RESULT_TIMEOUT;
    break;

    case WAIT_FAILED:
    // 函数调用失败，比如传递了一个无效的句柄
		ans = RESULT_TIMEOUT;
    break;
    }
  return ans;
}
result_t RFDevice::createThread() {
  
  _thread = CLASS_THREAD(RFDevice, cacheRecvData);

  if (_thread.getHandle() == 0) {
    m_isScanning = false;
    logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"RFDevice::createThread:%x",_serial);
    return RESULT_FAIL;
  }
  m_isScanning = true;
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"RFDevice::m_isScanning:%d",m_isScanning);
  return RESULT_OK;
}

void RFDevice::disconnect() {

	m_isScanning = false;
	_thread.terminate();
	if (_serial) {
		if (_serial->isOpen()) {
			_serial->flush();
			_serial->closePort();
		}
	}

	if (_serial) {
		delete _serial;
		_serial = NULL;
	}
}

bool RFDevice::isconnected() const { return m_isConnected; }
bool RFDevice::isscanning() const { return m_isScanning; }
result_t RFDevice::stopRecv(uint32_t timeout) {
  UNUSED(timeout);

  if (!m_isConnected) {
    return RESULT_FAIL;
  }

  // ScopedLocker l(_lock);
  // sendCommand(LIDAR_CMD_FORCE_STOP);
  // delay(5);
  // sendCommand(LIDAR_CMD_STOP);
  // delay(5);
  return RESULT_OK;
}

result_t RFDevice::sendCommand(uint8_t cmd, const void *payload, size_t payloadsize) {
  result_t ans;
  uint8_t pkt_header[10];
  cmd_packet *header = reinterpret_cast<cmd_packet *>(pkt_header);
  uint16_t checksum = 0;

  if (!m_isConnected) {
    return RESULT_FAIL;
  }

  header->header.syncByte = PROTOCOL_HEADER_FLAG;
  header->header.cmd_flag = cmd;
  header->header.dvrdressByte = 1;

  uint8_t szBuf[1024] = {0};
  uint16_t szBufSize = 0;
  uint8_t *pos = szBuf;

  if (payloadsize && payload) {
    uint16_t datasize = (uint16_t)payloadsize;
    EndianSwap((uint8_t *)&datasize, 0, 2);
    header->header.size = datasize;
    memcpy(pos, pkt_header, 5);
    pos += 5;
    szBufSize += 5;
    memcpy(pos, payload, payloadsize);
    pos += payloadsize;
    szBufSize += payloadsize;
  } else {
    header->header.size = 0;
    memcpy(pos, pkt_header, 5);
    pos += 5;
    szBufSize += 5;
  }

  checksum = modbusCRC16(szBuf, szBufSize);
  EndianSwap((uint8_t *)&checksum, 0, 2);
  memcpy(pos, &checksum, 2);
  szBufSize += 2;

  ans = sendData(szBuf, szBufSize);
  if(RESULT_OK != ans)
  {
     printf_s("&&&& cmd send:%d errcode:%d \n",cmd,ans);
    logInf::getInstance()->LogString2(LogLevelEnum::INFO,"&&&& cmd send:%d errcode:%d",cmd,ans);
   //printf("") 
   return ans;
  }
  

  // uint16_t sss = checksum;
  

//  ans = sendData((const uint8_t *)&checksum, 2);
//   if(RESULT_OK != ans)
//   {
//     // printf_s("&&222&& cmd send:%d errcode:%d \n",cmd,ans);

//     //return ans;
//   }
  //  uint8_t testchecksum[2] = { 0 };
  //  testchecksum[0] = 0x00;
  //  testchecksum[1] = 0x68;
  //  sendData((const uint8_t *)testchecksum, 2);
   logInf::getInstance()->LogString2(LogLevelEnum::INFO,"&&&&&&&&&&&&&&& cmd send:%d",cmd);
  printf_s("&&&&&&&&&&&&&&& cmd send:%d \n",cmd);
  return RESULT_OK;
}


result_t RFDevice::sendData(const uint8_t *data, size_t size) {
  
  if (!m_isConnected) {
    return RESULT_FAIL;
  }

  if (data == NULL || size == 0) {
    return RESULT_FAIL;
  }

  size_t r = 0;

  while (size) {
    r = _serial->writeData(data, size);
    // printf_s("&&&& sendData return:%d \n", r);

    if (r < 1) {
      return RESULT_FAIL;
    }

    size -= r;
    data += r;
  }

  return RESULT_OK;
}

//  std::ofstream outFile;
int RFDevice::cacheRecvData() {
  logInf::getInstance()->LogString2(LogLevelEnum::INFO, "cacheRecvData");
  result_t ans;
  int timeout_count = 0;
  // retryCount = 0;
  m_RecvBufferSize = 0;
  m_heartbeat_ts = getms();
  bool m_last_frame_valid = false;

  //  outFile.open("recv.dat", std::ios::out | std::ios::binary);

  while (m_isScanning) {
    cmd_packet cmd_packet_;
    uint16_t checksum = 0;
    ans = waitRecvHeader(&cmd_packet_.header, DATA_TIMEOUT / 2);
    
    if (IS_OK(ans)) {
      uint16_t datasize = cmd_packet_.header.size;
      EndianSwap((uint8_t *)&datasize, 0, 2);
      // logInf::getInstance()->LogString2(LogLevelEnum::INFO, "header info: syncByte:%d dvrdressByte:%d cmd_flag:%d size:%d",
      //   cmd_packet_.header.syncByte, cmd_packet_.header.dvrdressByte, cmd_packet_.header.cmd_flag, datasize);
      if(!checkPackageLength(cmd_packet_.header.cmd_flag,datasize))
      {
        logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "errro.cmd_flag:%d cmd_packet_.header.size:%d",cmd_packet_.header.cmd_flag,datasize);
        continue;
      }
      ans = waitForData(datasize, DATA_TIMEOUT / 2);
      if (IS_OK(ans)) {
        getData(cmd_packet_.data, datasize);
        // std::string data_hex_str = HexToString(&cmd_packet_.data[0], datasize);
        // logInf::getInstance()->LogString2(LogLevelEnum::INFO, "data info: %s", data_hex_str.c_str());

        uint16_t packLen = sizeof(device_ans_header) + datasize;
       
        checksum = modbusCRC16((uint8_t *)&cmd_packet_, packLen);
        EndianSwap((uint8_t *)&checksum, 0, 2);
        ans = waitForData(2, DATA_TIMEOUT / 2);
        if (IS_OK(ans)) {
          getData((uint8_t *)&cmd_packet_.checksum, 2);
        }
        else
        {
          logInf::getInstance()->LogString2(LogLevelEnum::EERROR,
                                            "read crc fail cmd_flag:%d cmd_packet_.header.size:%d",cmd_packet_.header.cmd_flag, 2);
        }
        //
        if (cmd_packet_.checksum == checksum) {
          SetEvent(hEvent);
          cmd_packet_.header.size = datasize;

          switch (cmd_packet_.header.cmd_flag) {
          case ACCU_CMD_ECG_DATA_GET: {
            static int datasizeCout = 0;
            datasizeCout++;
            if (datasizeCout % datasize * 5000 == 0) {
              // printf_s("&&&&&&&&&&&&&&& cmd recv:%d \n",cmd_packet_.header.cmd_flag);
              datasizeCout = 0;
            }

            //
            ScopedLocker l(_ecg_data_lock);
            ecg_infos.push(cmd_packet_);

          } break;
          case ACCU_CMD_IMP_DATA_GET: {
            ScopedLocker l(_imp_data_lock);
            imp_infos.push(cmd_packet_);
            static int datasizeCout2 = 0;
            datasizeCout2++;
            if (datasizeCout2 % datasize * 5000 == 0) {
              logInf::getInstance()->LogString2(LogLevelEnum::INFO,
                                            "&&&&&&&&&&&&&&&recv imp cmd send:%d", cmd_packet_.header.cmd_flag);
              datasizeCout2 = 0;
            }

          } break;
          case ACCU_CMD_RPLOT_DATA_GET: 
          case ACCU_CMD_CHIP_STATUS_GET: 
          case ACCU_CMD_ECG_STATUS_GET: 
          {
            std::string data_hex_str = HexToString(&cmd_packet_.data[0], datasize);
            logInf::getInstance()->LogString2(LogLevelEnum::INFO, "cmd:%d data info: %s", cmd_packet_.header.cmd_flag,data_hex_str.c_str());
            ScopedLocker l(_nomal_data_lock);
            nomal_infos.push(cmd_packet_);
          } break;
          default: {
            std::string data_hex_str = HexToString(&cmd_packet_.data[0], datasize);
            logInf::getInstance()->LogString2(LogLevelEnum::INFO, "cmd:%d data info: %s",cmd_packet_.header.cmd_flag, data_hex_str.c_str());
          } break;
          }
        } else {
            
          logInf::getInstance()->LogString2(LogLevelEnum::EERROR,
                                            "crc check fail cmd_flag:%d ans:%d",cmd_packet_.header.cmd_flag,ans);
        }
        has_device_header = false;
      }
      else{
        // logInf::getInstance()->LogString2(LogLevelEnum::EERROR,
        //                                     "read fail cmd_flag:%d ans:%d",cmd_packet_.header.cmd_flag,ans);
      }
    }
    if (!IS_OK(ans)) {
      if (IS_FAIL(ans) || timeout_count > DEFAULT_TIMEOUT_COUNT) {
        logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"errro.cmd_packet_.header.size timeout,handle:%0x ans:%d", this,ans);
      }
    }
  }
  //  outFile.close();
  m_isScanning = false;
  logInf::getInstance()->LogString2(LogLevelEnum::INFO,"RFDevice::cacheRecvData:exit");
  return RESULT_OK;
}
bool RFDevice::checkPackageLength( uint8_t cmd_flag, uint16_t size)
{
  bool isCheck = false;
  switch (cmd_flag) {
  case ACCU_CMD_READ_REGISTER_DATA_START: {
    if (size == 3)
      isCheck = true;
    else {
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:3",
                                        cmd_flag, size);
    }
  } break;
   case ACCU_CMD_WAVE_FILTER_DATA: {
    if (size == 28)
      isCheck = true;
    else {
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:28",
                                        cmd_flag, size);
    }
  } break;
  case ACCU_CMD_ECG_START: {
    if (size == 1)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:1",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_ECG_DATA_GET: {
    if (size < 255) {
      isCheck = true;
    }
    else
    {
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d",
        cmd_flag, size);
    }
  } break;
  case ACCU_CMD_ECG_STATUS_GET: {
    if (size == 4)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:4",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_RESPIRATION_DATA_GET: {
    if(size <255){
      isCheck = true;
    }
    else
    {
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d ",
                                        cmd_flag, size);
    }
    
  } break;
  case ACCU_CMD_PACEMAKE_DATA_GET: {
     if(size <255){
      isCheck = true;
    }
    else
    {
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d ",
                                        cmd_flag, size);
    }
  } break;
  case ACCU_CMD_DEBUGPRINT_DATA_GET: {
     if(size <255){
      isCheck = true;
    }
    else
    {
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d ",
                                        cmd_flag, size);
    }
  } break;
  case ACCU_CMD_RPLOT_DATA_GET: {
    if (size == 26)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:26",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_IMP_START: {
    if (size == 1)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:1",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_IMP_DATA_GET: {
    if (size == 5)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:5",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_IMP_STATUS_GET: {
    if (size == 4)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:4",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_CHIP_STATUS_GET: {
    if (size == 10)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:10",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_WORK_PARAMETER_DATA_SET: {
    if (size == 1)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:1",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_WORK_PARAMETER_DATA_GET: {
    if (size == 32)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:32",
                                        cmd_flag, size);
  } break;
  case ACCU_CMD_PRODUCT_INFORMATION_DATA_GET: {
    if (size == 80)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:80",
        cmd_flag, size);
  } break;
  case ACCU_CMD_PRODUCT_INFORMATION_DATA_SET: {
    if (size == 1)
      isCheck = true;
    else
      logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "cmd:%d packet.size:%d need:1",
                                        cmd_flag, size);

  } break;
  default: {

  } break;
  }

  return isCheck;
}
std::string RFDevice::HexToString(std::uint8_t *in, int len)
{
  int i;
  std::uint8_t inChar, hi, lo;
  std::string s;
  std::string h16;
  std::string l16;

  for (i = 0; i < len; i++)
  {
    inChar = in[i];

    hi = (inChar & 0xF0) >> 4;
    if (hi > 9)
      hi = 'A' + (hi - 0x0A);
    else
      hi += 0x30;
    s.push_back(hi);

    lo = inChar & 0x0F;
    if (lo > 9)
      lo = 'A' + (lo - 0x0A);
    else
      lo += 0x30;
    s.push_back(lo);
  }

  return s;
}

int RFDevice::disposeECGData() {
  static int nCounttime = 0;
  result_t ans;
  while (is_ecg_dispose_thread) {
    bool isEmpty = false;
    {
      ScopedLocker l(_ecg_data_lock);
      isEmpty = ecg_infos.empty();
    }
    if (isEmpty) {
        delay(4);
        continue;
     }
    cmd_packet cmd_packet_;
    {
      ScopedLocker l(_ecg_data_lock);
     
      cmd_packet_ = ecg_infos.front();
      ecg_infos.pop();
    }

    std::vector<ECGData> vectorEcgData;
    uint8_t *pos = cmd_packet_.data;
    uint16_t flag = *(uint16_t *)pos;  pos += 2;
    uint16_t frame = flag>>14;
    uint16_t channnelF = flag;
    EndianSwap((uint8_t *)&channnelF, 0, 2);

    uint32_t serialNo = *(uint32_t *)pos;
    pos += 4;
    EndianSwap((uint8_t *)&serialNo, 0, 4);

    uint8_t dataCount = *(uint8_t *)pos;
    pos += 1;

    for (uint8_t i = 0; i < dataCount; i++) { //第n组数据
      for (uint8_t n = 0; n < 10; n++) {
        if ((channnelF >> n) & 1) {
          ECGData temp;
          temp.frame = frame;
          temp.serialNo = serialNo;
          temp.channel = n;
          uint32_t ecgValue = *(uint32_t *)pos;
          pos += 4;
          temp.ecgValue = switchValue(u8Arry2float((uint8_t *)&ecgValue));
          if(m_nChannel-1 != n)
          {
            //printf_s("&&&&&&&&&&&&&&& m_nChannel :%d \n",m_nChannel);
            continue;
          } 
          //vectorEcgData.push_back(temp);
          calcEcgData* calEcgData = GetCalEcg(temp.channel);
          if(calEcgData)
          {
            if(serialNo == 0){
            calEcgData->resetCalc();
            calEcgData->init420();
            logInf::getInstance()->LogString2(LogLevelEnum::INFO, "&&&&&&&&&&&&&&& calEcgData->resetCalc():%d \n",temp.channel);
           // printf_s("&&&&&&&&&&&&&&& calEcgData->resetCalc():%d \n",temp.channel);
            }
         
            // auto[nResult, vectorEcgData]  = calEcgData->addEcgData(temp);
            auto[nResult, vectorEcgData]  = calEcgData->addEcgDataTo420(temp);
            if(nResult == 1) 
            {
              if(temp.serialNo % 250 == 0){
                logInf::getInstance()->LogString2(LogLevelEnum::INFO, "recveEcgcound:%d &&&&&&&&&&&&&&& lead:%d serialNo:%d \n",dataCount,temp.channel,temp.serialNo);
              }
               
              // printf_s("&&&&&&&&&&&&&&& recve->ecgdata size:%d \n",vectorEcgData.size());
              //  logInf::getInstance()->LogString2(LogLevelEnum::INFO, "&&&&&&&&&&&&&&& recve->ecgdata size:%d \n",vectorEcgData.size());
                std::vector<ECGData> *myvec = static_cast<std::vector<ECGData> *>(&vectorEcgData);
                m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, myvec, vectorEcgData.size(), m_userId);
                vectorEcgData.clear();
            }
          }
        }
      }
    }

  }
  is_ecg_dispose_thread = false;
  return true;
}
calcEcgData* RFDevice::GetCalEcg(uint8_t nchannle) 
{
  calcEcgData* calEcgData_ptr = NULL;
  {
    ScopedLocker l(_mapEcgData_lock);
     std::map<uint8_t,calcEcgData*>::iterator it = mapEcgData.find(nchannle); 
    if(it == mapEcgData.end())
    {
      calEcgData_ptr = new calcEcgData();
      mapEcgData.insert(std::pair<uint8_t,calcEcgData*>(nchannle,calEcgData_ptr));
    }
    else{
      calEcgData_ptr = it->second;
    }
    return calEcgData_ptr;
  }

 
}
int RFDevice::disposeIMPData() {
  while (is_imp_dispose_thread) {
    bool isEmpty = false;
    {
       ScopedLocker l(_imp_data_lock);
       isEmpty = imp_infos.empty();
    }
   if (isEmpty) {
        delay(200);
        continue;
    }
    cmd_packet cmd_packet_;
    {
      ScopedLocker l(_imp_data_lock);
     
      cmd_packet_ = imp_infos.front();
      imp_infos.pop();
    }

    // uint32_t datasize = sizeof(cmd_packet) + cmd_packet_.datasize;

    std::vector<IMPData> vectorImpData;
    uint8_t *pos = cmd_packet_.data;

    for (int i = 0; i < (cmd_packet_.header.size / 5); i++) {
      IMPData temp;
      temp.channel = *(uint8_t *)pos;
      pos += 1;
      // EndianSwap((uint8_t*)&impValue, 0, 4);
      temp.impValue = u8Arry2float(pos);
      pos += 4;

      if (temp.channel < 0 || temp.channel > 9) {
        logInf::getInstance()->LogString2(LogLevelEnum::EERROR, "temp.channel out of range %d", temp.channel);
        continue;
      }
      vectorImpData.push_back(temp);
    }
    std::vector<IMPData> *myvec = static_cast<std::vector<IMPData> *>(&vectorImpData);
    m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, myvec, vectorImpData.size(), m_userId);
    //printf_s("&&&&&&&&&&&&&&& dispose imp cmd send:%d  size:%d vectorImpData:%d\n",cmd_packet_.header.cmd_flag,imp_infos.size(),vectorImpData.size());
    // logInf::getInstance()->LogString2(LogLevelEnum::INFO,
    //   "&&&&&&&&&&&&&&& dispose imp cmd send:%d  size:%d vectorImpData:%d",cmd_packet_.header.cmd_flag,imp_infos.size(),vectorImpData.size());
    // vectorImpData.clear();
    /* if(cmd_packet_.data != NULL)
     {
         delete[] cmd_packet_.data;
         cmd_packet_.data = NULL;
     }*/
  }
  is_imp_dispose_thread = false;
  return true;
}
int RFDevice::disposeNomalData() {
  while (is_nomal_dispose_thread) {
   
    bool isEmpty = false;
    {
       ScopedLocker l(_nomal_data_lock);
       isEmpty = nomal_infos.empty();
    }
    if(isEmpty)
    {
        delay(200);
        continue;
    }
    cmd_packet cmd_packet_;
    {
      ScopedLocker l(_nomal_data_lock);
      cmd_packet_ = nomal_infos.front();
      nomal_infos.pop();
    }

   
   
    switch (cmd_packet_.header.cmd_flag) 
    {
      case ACCU_CMD_RPLOT_DATA_GET: {
        disposeRpotData(cmd_packet_);
      } break;
      case ACCU_CMD_ECG_STATUS_GET: {
        disposeECGStatusData(cmd_packet_);
      }break;
      case ACCU_CMD_CHIP_STATUS_GET:
      { 
        disposeChipData(cmd_packet_);
      }break;
    }

  }
  is_nomal_dispose_thread = false;
  return true;
}
int RFDevice::disposeRpotData(cmd_packet cmd_packet_)
{
  std::vector<RPlotData> vectorRPlotData;
  uint8_t *pos = cmd_packet_.data;
  uint16_t flag = *(uint16_t *)pos;  pos += 2;
  uint16_t frame = flag>>14;
  uint16_t channnelF = flag;
  EndianSwap((uint8_t *)&channnelF, 0, 2);

  for (uint8_t n = 0; n < 10; n++) {
    if ((channnelF >> n) & 1) {
      RPlotData temp;
      temp.channel = n;
      temp.frame = frame;
      uint32_t R_index = *(uint32_t *)pos; pos += 4;
      temp.R_index = u8Arry2Int((uint8_t *)&R_index);
      

      uint32_t RPlot_position = *(uint32_t *)pos; pos += 4;
      temp.RPlot_position = u8Arry2Int((uint8_t *)&RPlot_position);

      // uint32_t RPlot_position_value = *(uint32_t *)pos; pos += 4;
      // temp.RPlot_position_value = switchValue(u8Arry2float((uint8_t *)&RPlot_position_value));

      uint32_t rrInterval = *(uint32_t *)pos; pos += 4;
      temp.rrInterval = u8Arry2float((uint8_t *)&rrInterval);

        uint32_t rWaveDelayCount = *(uint32_t *)pos; pos += 4;
      temp.rWaveDelayCount = u8Arry2float((uint8_t *)&rWaveDelayCount);

      uint32_t ecg_position = *(uint32_t *)pos; pos += 4;
      temp.ecg_position = u8Arry2Int((uint8_t *)&ecg_position);

      //   uint32_t ecg_position_Value = *(uint32_t *)pos; pos += 4;
      // temp.ecg_position_Value = switchValue(u8Arry2float((uint8_t *)&ecg_position_Value));

        uint32_t standard_deviation = *(uint32_t *)pos; pos += 4;
      temp.standard_deviation = u8Arry2float((uint8_t *)&standard_deviation);
      vectorRPlotData.push_back(temp);

    }
  }
  std::vector<RPlotData> *myvec = static_cast<std::vector<RPlotData> *>(&vectorRPlotData);
  m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, myvec, vectorRPlotData.size(), m_userId);
  vectorRPlotData.clear();
  return true;
}
int RFDevice::disposeECGStatusData(cmd_packet cmd_packet_)
{
    DEVICE_ECG_STATUS ecg_status = {0};
    uint8_t *pos = cmd_packet_.data;
    ecg_status.STATUS = pos[0]; pos++;
    for (uint8_t n = 0; n < 6; n++)
    {
      if ((ecg_status.STATUS >> n) & 1) 
      {
        RMESSAGEData messagedata ;
        messagedata.messageType = DriverError::BlockError;
        std::string sname = "ECG_staus error"+std::to_string(n);
        strcpy_s(messagedata.mesageName,sname.c_str());
         std::string sname2 = "error_code ecg_status.STATUS error"+std::to_string(n);
        strcpy_s(messagedata.mesagedes,sname2.c_str());
        printf_s("error_code ecg_status.STATUS:%d \n",n);
        // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
        logInf::getInstance()->LogString2(LogLevelEnum::INFO,"error_code ecg_status.STATUS:%d",n);
      }
    }

    ecg_status.ADC_STATUS = pos[0]; pos++;
    for (uint8_t n = 0; n < 3; n++)
    {
        if ((ecg_status.ADC_STATUS >> n) & 1) 
        {
        RMESSAGEData messagedata ;
        messagedata.messageType = DriverError::BlockError;
        std::string sname = "ADC_STATUS error"+std::to_string(n);
        strcpy_s(messagedata.mesageName,sname.c_str());

         std::string sname2 = "error_code ecg_status.ADC_STATUS error"+std::to_string(n);
        strcpy_s(messagedata.mesagedes,sname2.c_str());

        logInf::getInstance()->LogString2(LogLevelEnum::INFO,"error_code ecg_status.STATUS:%d",n);
        printf_s("error_code ecg_status.ADC_STATUS:%d \n",n);
        // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
        }
    }

    ecg_status.DCLO_HI_STATUS = pos[0]; pos++;
    for (uint8_t n = 0; n < 4; n++)
    {
        if ((ecg_status.DCLO_HI_STATUS >> n) & 1) 
        {
          RMESSAGEData messagedata ;
          messagedata.messageType = DriverError::BlockError;
          std::string sname = "DCLO_HI_STATUS error"+std::to_string(n);
          strcpy_s(messagedata.mesageName,sname.c_str());

          std::string sname2 = "error_code ecg_status.DCLO_HI_STATUS error"+std::to_string(n);
          strcpy_s(messagedata.mesagedes,sname2.c_str());

          logInf::getInstance()->LogString2(LogLevelEnum::INFO,"error_code ecg_status.DCLO_HI_STATUS:%d",n);
          printf_s("error_code ecg_status.DCLO_HI_STATUS:%d \n",n);
          // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
        }
    }
    ecg_status.DCLO_LO_STATUS = pos[0]; pos;
    for (uint8_t n = 0; n < 4; n++)
    {
        if ((ecg_status.DCLO_LO_STATUS >> n) & 1) 
        {
            RMESSAGEData messagedata ;
            messagedata.messageType = DriverError::BlockError;
            std::string sname = "DCLO_LO_STATUS error"+std::to_string(n);
            strcpy_s(messagedata.mesageName,sname.c_str());

            std::string sname2 = "error_code ecg_status.DCLO_LO_STATUS error"+std::to_string(n);
            strcpy_s(messagedata.mesagedes,sname2.c_str());

            logInf::getInstance()->LogString2(LogLevelEnum::INFO,"error_code ecg_status.DCLO_LO_STATUS:%d",n);
            printf_s("error_code ecg_status.DCLO_LO_STATUS:%d \n",n);
            // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
        }
    }
  return true;
}

int RFDevice::disposeChipData(cmd_packet cmd_packet_)
{
  uint8_t *pos = cmd_packet_.data;
  uint16_t status = *(uint16_t *)pos;  pos += 1;
  DEVICE_ECG_STATUS ecg_status = {0};
   ecg_status.STATUS = pos[0]; pos++;
    for (uint8_t n = 0; n < 6; n++)
    {
      if ((ecg_status.STATUS >> n) & 1) 
      {
        RMESSAGEData messagedata ;
        messagedata.messageType = DriverError::BlockError;
        std::string sname = "ECG_staus error"+std::to_string(n);
        strcpy_s(messagedata.mesageName,sname.c_str());
        std::string sname2 = "error_code ecg_status.STATUS error"+std::to_string(n);
        strcpy_s(messagedata.mesagedes,sname2.c_str());
        
        printf_s("error_code ecg_status.STATUS:%d \n",n);
        logInf::getInstance()->LogString2(LogLevelEnum::INFO,"error_code ecg_status.STATUS:%d",n);
        // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
      }
    }

    ecg_status.ADC_STATUS = pos[0]; pos++;
    for (uint8_t n = 0; n < 6; n++)
    {
        if ((ecg_status.ADC_STATUS >> n) & 1) 
        {
        RMESSAGEData messagedata ;
        messagedata.messageType = DriverError::BlockError;
        std::string sname = "ADC_STATUS error"+std::to_string(n);
        strcpy_s(messagedata.mesageName,sname.c_str());

         std::string sname2 = "error_code ecg_status.ADC_STATUS error"+std::to_string(n);
            strcpy_s(messagedata.mesagedes,sname2.c_str());

        printf_s("error_code ecg_status.ADC_STATUS:%d \n",n);
        logInf::getInstance()->LogString2(LogLevelEnum::INFO,"error_code ecg_status.ADC_STATUS:%d",n);
        // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
        }
    }

    ecg_status.DCLO_HI_STATUS = pos[0]; pos++;
    for (uint8_t n = 0; n < 5; n++)
    {
        if ((ecg_status.DCLO_HI_STATUS >> n) & 1) 
        {
          RMESSAGEData messagedata ;
          messagedata.messageType = DriverError::BlockError;
          std::string sname = "DCLO_HI_STATUS error"+std::to_string(n);
          strcpy_s(messagedata.mesageName,sname.c_str());

          std::string sname2 = "error_code ecg_status.DCLO_HI_STATUS error"+std::to_string(n);
          strcpy_s(messagedata.mesagedes,sname2.c_str());

          printf_s("error_code ecg_status.DCLO_HI_STATUS:%d \n",n);
          logInf::getInstance()->LogString2(LogLevelEnum::INFO,"error_code ecg_status.DCLO_HI_STATUS:%d",n);
          // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
        }
    }
    ecg_status.DCLO_LO_STATUS = pos[0]; pos++;
    for (uint8_t n = 0; n < 5; n++)
    {
        if ((ecg_status.DCLO_LO_STATUS >> n) & 1) 
        {
              RMESSAGEData messagedata ;
            messagedata.messageType = DriverError::BlockError;
            std::string sname = "DCLO_LO_STATUS error"+std::to_string(n);
            strcpy_s(messagedata.mesageName,sname.c_str());

            std::string sname2 = "error_code ecg_status.DCLO_LO_STATUS error"+std::to_string(n);
            strcpy_s(messagedata.mesagedes,sname2.c_str());

            printf_s("error_code ecg_status.DCLO_LO_STATUS:%d \n",n);
            logInf::getInstance()->LogString2(LogLevelEnum::INFO,"error_code ecg_status.DCLO_LO_STATUS:%d",n);
            // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
        }
    }
  uint16_t imp_status = *(uint16_t *)pos;  pos += 1;
  uint32_t adc_status = *(uint32_t *)pos; ;
  DEVICE_IMP_STATUS device_imp_status = {0};
  device_imp_status.ADC_Value = pos[0];  pos++;

  if ((device_imp_status.ADC_Value >> 5) & 1) 
  {
      RMESSAGEData messagedata ;
      messagedata.messageType = DriverError::BlockError;
      std::string sname = "DEVICE_IMP_STATUS minimum value error";
      strcpy_s(messagedata.mesageName,sname.c_str());

      std::string sname2 = "DEVICE_IMP_STATUS minimum value error";
      strcpy_s(messagedata.mesagedes,sname2.c_str());

      printf_s("DEVICE_IMP_STATUS minimum value error \n");
      logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DEVICE_IMP_STATUS minimum value error");
      // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
  }
  if ((device_imp_status.ADC_Value >> 6) & 1) 
  {
      RMESSAGEData messagedata ;
      messagedata.messageType = DriverError::BlockError;
      std::string sname = "DEVICE_IMP_STATUS Maximum value error";
      strcpy_s(messagedata.mesageName,sname.c_str());

      std::string sname2 = "DEVICE_IMP_STATUS Maximum value error";
      strcpy_s(messagedata.mesagedes,sname2.c_str());

      printf_s("DEVICE_IMP_STATUS Maximum value error \n");
      logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DEVICE_IMP_STATUS Maximum value error");
      // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
  }  
    if ((device_imp_status.ADC_Value >> 7) & 1) 
  {
      RMESSAGEData messagedata ;
      messagedata.messageType = DriverError::BlockError;
      std::string sname = "DEVICE_IMP_STATUS Adc DELTA Ready";
      strcpy_s(messagedata.mesageName,sname.c_str());

      std::string sname2 = "DEVICE_IMP_STATUS DELTA Ready";
      strcpy_s(messagedata.mesagedes,sname2.c_str());

      printf_s("DEVICE_IMP_STATUS DELTA Ready \n");
      logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DEVICE_IMP_STATUS DELTA Ready");
      // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
  }  

  device_imp_status.ADC_Sequencer = pos[0];  pos++;
  if ((device_imp_status.ADC_Sequencer >> 2) & 1) 
  {
      RMESSAGEData messagedata ;
      messagedata.messageType = DriverError::BlockError;
      std::string sname = "DEVICE_IMP_STATUS qequencer";
      strcpy_s(messagedata.mesageName,sname.c_str());

      std::string sname2 = "DEVICE_IMP_STATUS qequencer";
      strcpy_s(messagedata.mesagedes,sname2.c_str());

      printf_s("DEVICE_IMP_STATUS qequencer \n");
      logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DEVICE_IMP_STATUS qequencer");
      // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
  }  
  
  device_imp_status.Outlier = pos[0];  pos++;
  if ((device_imp_status.Outlier) & 1) 
  {
      RMESSAGEData messagedata ;
      messagedata.messageType = DriverError::BlockError;
      std::string sname = "DEVICE_IMP_STATUS Data FIFO FULL";
      strcpy_s(messagedata.mesageName,sname.c_str());

      std::string sname2 = "DEVICE_IMP_STATUS Data FIFO FULL";
      strcpy_s(messagedata.mesagedes,sname2.c_str());

      logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DEVICE_IMP_STATUS Data FIFO FULL");
      // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
  }  
      
  if ((device_imp_status.Outlier >> 6) & 1) 
  {
      RMESSAGEData messagedata ;
      messagedata.messageType = DriverError::BlockError;
      std::string sname = "DEVICE_IMP_STATUS Data Outlier Int";
      strcpy_s(messagedata.mesageName,sname.c_str());

      std::string sname2 = "DEVICE_IMP_STATUS Data Outlier Int";
      strcpy_s(messagedata.mesagedes,sname2.c_str());

      printf_s("DEVICE_IMP_STATUS Data Outlier Int \n");
      logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DEVICE_IMP_STATUS Data Outlier Int");
      // m_callback(m_DeviceId, cmd_packet_.header.cmd_flag, &messagedata, sizeof(RMESSAGEData), m_userId);
  } 
  return true;
}
result_t RFDevice::startECG(uint32_t timeout) {

  UNUSED(timeout);
  if (!m_isConnected) {
    return RESULT_FAIL;
  }
  result_t ans = RESULT_FAIL;
  char status = 1;
  size_t size_ = sizeof(status);
  ScopedLocker l(_lock);
  if ((ans = sendCommand(ACCU_CMD_ECG_START, &status, sizeof(char))) != RESULT_OK) {
    return ans;
  }
  return ans;
}

result_t RFDevice::stopECG(uint32_t timeout) {

  UNUSED(timeout);
  if (!m_isConnected) {
    return RESULT_FAIL;
  }
  result_t ans = RESULT_FAIL;
  char status = 0;
  size_t size_ = sizeof(status);
  ScopedLocker l(_lock);
  if ((ans = sendCommand(ACCU_CMD_ECG_START, &status, sizeof(char))) != RESULT_OK) {
    return ans;
  }
  return ans;
}
result_t RFDevice::resetIMP(uint32_t timeout) {

  UNUSED(timeout);
  if (!m_isConnected) {
    return RESULT_FAIL;
  }
  result_t ans = RESULT_FAIL;
  char status = 2;
  size_t size_ = sizeof(status);
  ScopedLocker l(_lock);
  if ((ans = sendCommand(ACCU_CMD_IMP_START, &status, sizeof(char))) != RESULT_OK) {
    return ans;
  }
  return ans;
}


result_t RFDevice::startIMP(uint32_t timeout) {

  UNUSED(timeout);
  if (!m_isConnected) {
    return RESULT_FAIL;
  }
  result_t ans = RESULT_FAIL;
  char status = 1;
  size_t size_ = sizeof(status);
  ScopedLocker l(_lock);
  if ((ans = sendCommand(ACCU_CMD_IMP_START, &status, sizeof(char))) != RESULT_OK) {
    return ans;
  }
  return ans;
}

result_t RFDevice::stopIMP(uint32_t timeout) {
  UNUSED(timeout);
  if (!m_isConnected) {
    return RESULT_FAIL;
  }
  result_t ans = RESULT_FAIL;
  char status = 0;
  size_t size_ = sizeof(status);
  ScopedLocker l(_lock);
  if ((ans = sendCommand(ACCU_CMD_IMP_START, &status, sizeof(char))) != RESULT_OK) {
    return ans;
  }
  return ans;
}
bool RFDevice::setdeviceopt(int optcount, const void *optval, int optlen) {
  result_t ans = RESULT_FAIL;
  int count = sizeof(WORK_PARAMETER)/ optlen;
  if (optcount != count)
    return ans;

  char szBuf[1024] = {0};
  int szBufSize = 0;
  char *pos = szBuf;
  pos[0] = optcount;
  pos++;
  szBufSize++;
  memcpy(pos, optval, optlen);
  szBufSize += optlen;

  ScopedLocker l(_lock);
  if ((ans = sendCommand(ACCU_CMD_WORK_PARAMETER_DATA_SET, szBuf, szBufSize)) != RESULT_OK) {
    return ans;
  }

  return RESULT_OK;
}
bool RFDevice::setdeviceopt_RWaveLead(int lead) 
{
  m_nChannel = lead;
  result_t ans = RESULT_FAIL;
  WORK_PARAMETER parame;
  parame.parameter_type = 1;

  EndianSwap((uint8_t *)&lead,0,4);
  memcpy(&parame.parameter_data,&lead,4);

  calcEcgData* calEcgData = GetCalEcg(lead);
  if(calEcgData)  calEcgData->init420();
   

 return setdeviceopt(1,&parame,sizeof(parame));
 
}
bool RFDevice::setdeviceopt_Delaytime(int delaytime) 
{
  result_t ans = RESULT_FAIL;
  WORK_PARAMETER parame;
  parame.parameter_type = 2;

  EndianSwap((uint8_t *)&delaytime,0,4);
  memcpy(&parame.parameter_data,&delaytime,4);
 return setdeviceopt(1,&parame,sizeof(parame));
}

bool RFDevice::setdeviceopt_RRInterval(float RRInterval) 
{
  result_t ans = RESULT_FAIL;
  WORK_PARAMETER parame;
  parame.parameter_type = 4;

  float2u8Arry(parame.parameter_data,&RRInterval);
 return setdeviceopt(1,&parame,sizeof(parame));
}

bool RFDevice::getdeviceopt(int optname, const void *optval, int &optlen) {
  result_t ans = RESULT_FAIL;
  char status = 0;
  size_t size_ = sizeof(status);
  ScopedLocker l(_lock);
  if ((ans = sendCommand(ACCU_CMD_WORK_PARAMETER_DATA_GET)) != RESULT_OK) {
    return ans;
  }

  return RESULT_OK;
}

result_t RFDevice::getDeviceSHIPInfo(SHIP_INFO &info, uint32_t timeout) {
  UNUSED(timeout);
  if (!m_isConnected) {
    return RESULT_FAIL;
  }
  result_t ans = RESULT_FAIL;
  char status = 0;
  size_t size_ = sizeof(status);

  ScopedLocker l(_lock);
  if ((ans = sendCommand(ACCU_CMD_WORK_PARAMETER_DATA_SET)) != RESULT_OK) {
    return ans;
  }
  return RESULT_OK;
}

result_t RFDevice::checkAutoConnecting(bool serialError) {
  result_t ans = RESULT_FAIL;
  isAutoconnting = true;

  if (m_driverErrno != BlockError) {
    setDriverError(TimeoutError);
    logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"BlockError");
  }

  while (isAutoReconnect && isAutoconnting) {
    {
      ScopedLocker l(_cmd_lock);

      if (_serial) {
        if (_serial->isOpen() || m_isConnected) {
          size_t buffer_size = _serial->available();
          m_BufferSize += buffer_size;

          if (m_BufferSize && m_BufferSize % 7 == 0) {
            setDriverError(BlockError);
            logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"BlockError 11111");
          } else {
            if (buffer_size > 0 || m_BufferSize > 0) {
              if (m_driverErrno != BlockError) {
                setDriverError(TrembleError);
                logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"TrembleError 22222");
              }
            } else {
              setDriverError(NotBufferError);
              logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"NotBufferError 33333");
            }
          }

          if ((retryCount % 2 == 1) || serialError) {
            m_isConnected = false;
            _serial->closePort();
            logInf::getInstance()->LogString2(LogLevelEnum::INFO,"closePort 44444");
            delete _serial;
            _serial = NULL;
          } else {
            m_BufferSize -= buffer_size;
          }
        }
      }
    }
    retryCount++;

    if (retryCount > 50) {
      retryCount = 50;
    }

    int tempCount = 0;

    while (isAutoReconnect && isscanning() && tempCount < retryCount) {
      delay(200);
      tempCount++;
    }

    tempCount = 0;
    int retryConnect = 0;

    while (isAutoReconnect && connectDevice(serial_port.c_str(), m_baudrate) != RESULT_OK) {
      retryConnect++;

      if (retryConnect > 25) {
        retryConnect = 25;
      }

      tempCount = 0;
      setDriverError(NotOpenError);

      while (isAutoReconnect && isscanning() && tempCount < retryConnect) {
        delay(200);
        tempCount++;
      }
    }

    if (!isAutoReconnect) {
      m_isScanning = false;
      return RESULT_FAIL;
    }

    if (isconnected()) {
      delay(50);

      if (m_driverErrno != BlockError) {
        SHIP_INFO infostatus;
        ans = getDeviceSHIPInfo(infostatus);

        if (!IS_OK(ans)) {
          logInf::getInstance()->LogString2(LogLevelEnum::INFO,"getDeviceSHIPInfo 5555");
          ans = getDeviceSHIPInfo(infostatus);
        }

        if (!IS_OK(ans)) {
          logInf::getInstance()->LogString2(LogLevelEnum::INFO,"setDriverError 66666");
          setDriverError(DeviceNotFoundError);
          continue;
        }
      }

      {
        ScopedLocker l(_cmd_lock);
        // ans = startAutoScan();

        // if (!IS_OK(ans)) {
        //   ans = startAutoScan();
        // }
      }

      if (IS_OK(ans)) {
        isAutoconnting = false;

        if (getDriverError() == DeviceNotFoundError) {
          setDriverError(NoError);
          logInf::getInstance()->LogString2(LogLevelEnum::INFO,"NoError 77777");
        }

        return ans;
      } else {
        setDriverError(DeviceNotFoundError);
        logInf::getInstance()->LogString2(LogLevelEnum::INFO,"DeviceNotFoundError 8888");
      }
    }
  }

  return RESULT_FAIL;
}

result_t RFDevice::waitRecvHeader(device_ans_header *header, uint32_t timeout) {
  int recvPos = 0;
  uint32_t startTs = getms();
  uint8_t recvBuffer[sizeof(device_ans_header)] = {0};
  uint8_t *headerBuffer = reinterpret_cast<uint8_t *>(header);
  uint32_t waitTime = 0;
  has_device_header = false;
  last_device_byte = 0x00;

  while ((waitTime = getms() - startTs) <= timeout) {
    if(recvPos>sizeof(device_ans_header))
    {
       logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"recvPos:%d ", recvPos);
    }

    size_t remainSize = sizeof(device_ans_header) - recvPos;

    size_t recvSize = 0;
    result_t ans = waitForData(remainSize, timeout - waitTime, &recvSize);

    if (!IS_OK(ans)) {
      //logInf::getInstance()->LogString2(LogLevelEnum::EERROR,"RFDevice::waitRecvHeader->waitForData fail ans:%d", ans);
      return ans;
    }

    if (recvSize > remainSize) {
      recvSize = remainSize;
    }

    ans = getData(recvBuffer, recvSize);

    if (IS_FAIL(ans)) {
      logInf::getInstance()->LogString2(LogLevelEnum::INFO,"FDevice::waitRecvHeader->getData fail ans:%d", ans);
      return RESULT_FAIL;
    }

    for (size_t pos = 0; pos < recvSize; ++pos) {
      uint8_t currentByte = recvBuffer[pos];
      if (recvPos == 0 && currentByte != PROTOCOL_HEADER_FLAG) {
        last_device_byte = currentByte;
        recvPos = 0;
        continue;
      }
      headerBuffer[recvPos++] = currentByte;

      if (recvPos == sizeof(device_ans_header)) {
        return RESULT_OK;
      }
    }
  }

  return RESULT_FAIL;
}

result_t RFDevice::waitForData(size_t data_count, uint32_t timeout, size_t *returned_size) {
	if (!m_isConnected) {
		return RESULT_FAIL;
	}
	size_t length = 0;

  if (returned_size == NULL) {
    returned_size = (size_t *)&length;
  }
  if (_serial)
  {
	  return (result_t)_serial->waitfordata(data_count, timeout, returned_size);
  }
  return RESULT_FAIL;

}

result_t RFDevice::getData(uint8_t *data, size_t size) {
  if (!m_isConnected) {
    return RESULT_FAIL;
  }

  size_t r = 0;

  while (size) {
    r = _serial->readData(data, size);
    //  outFile.write((char*)&data, size);
    if (r < 1) {
      return RESULT_FAIL;
    }

    size -= r;
    data += r;
  }

  return RESULT_OK;
}

void RFDevice::flushSerial() {
  if (!m_isConnected) {
    return;
  }

  size_t len = _serial->available();

  if (len) {
    _serial->readSize(len);
  }

  delay(20);
}
result_t RFDevice::SendTest(uint8_t cmd, uint8_t *test, int len) {
  result_t ans = RESULT_FAIL;
  if ((ans = sendCommand(cmd, test, len)) != RESULT_OK) {
    return ans;
  }
  return ans;
}
float RFDevice::switchValue(float value)
{
  // return value*1000.0*(4*1.8/1.4)/16777215;
  return value;
}

} // namespace accu