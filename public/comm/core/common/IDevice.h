#pragma once
#include <core/base/v8stdint.h>
#include "comm_def.h"
#include <core/network/ActiveSocket.h>
#include <core/serial/common.h>
#include <core/base/thread.h>
#include <core/base/locker.h>
#include "comm_sdk.h"

namespace accu {
namespace core {
  using namespace base;
namespace common {

  
const unsigned char auchCRCHi[]=
{
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};


const unsigned char auchCRCLo[] =
{
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};

struct StreamDataInfo {
  PUCHAR data_ptr = nullptr;
  uint32_t data_len = 0;

  StreamDataInfo(PUCHAR _ptr, uint32_t _len) {
    data_ptr = _ptr;
    data_len = _len;
  }
};

class IDevice {
public:
  IDevice(DEVICMODE device_mode)
  :m_deviceMode(device_mode)
  {
    static long deviceId = 0;
    deviceId++;
    m_DeviceId = deviceId;
    if(deviceId > 0xffffffff)
    {
      deviceId=0;
    }
  }
  virtual ~IDevice() {}
public:
  // virtual void SetDeviceMode(DEVICMODE mode) { m_deviceMode = mode; }
   virtual bool Initialize(MsgCallback *callback,void* userId)
   {
     m_callback = callback;
     m_userId = userId;
     return true;
   }
  virtual bool UnInitialize()
  {
     return true;
  }

  virtual long getDeviceId(){return m_DeviceId;}

  virtual int getDeviceType() const { return m_DeviceType;}
   /**
   * @brief Connecting device \n
   * After the connection if successful, you must use ::disconnect to close
   * @param[in] port_path    serial port
   * @param[in] baudrate    serial baudrate
   * @return connection status
   * @retval 0     success
   * @retval < 0   failed
   * @note After the connection if successful, you must use ::disconnect to close
   * @see function ::IDevice::disconnect ()
   */
  virtual result_t connectDevice(const char *port_path, uint32_t baudrate = 8000) = 0;
    /*!
  * @brief Disconnect the LiDAR.
  */
  virtual void disconnect() = 0;

    /**
   * @brief Is it connected to the device \n
   * @return connection status
   * @retval true     connected
   * @retval false    Non-connected
   */
  virtual bool isconnected() const = 0;
  /**
   * @brief Is the device in the scan \n
   * @return scanning status
   * @retval true     scanning
   * @retval false    non-scanning
   */
  virtual bool isscanning() const = 0;

    /**
   * @brief Turn on ECG \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
  virtual result_t startECG(uint32_t timeout = DEFAULT_TIMEOUT) = 0;

  /**
   * @brief turn off ECG \n
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
  virtual result_t stopECG(uint32_t timeout = DEFAULT_TIMEOUT) = 0;

  /**
   * @brief Turn on IMP \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
  virtual result_t resetIMP(uint32_t timeout = DEFAULT_TIMEOUT) = 0;
  
     /**
   * @brief Turn on IMP \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
  virtual result_t startIMP(uint32_t timeout = DEFAULT_TIMEOUT) = 0;

  

 
  /**
   * @brief turn off IMP \n
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
  virtual result_t stopIMP(uint32_t timeout = DEFAULT_TIMEOUT) = 0;

  /**
     * @param optval         option value
   * - std::string(or char*)
   * - int
   * - bool
   * - float
   * @param optlen         option length
   * - data type size
   * @return true if the Property is set successfully, otherwise false.
   * @see 
   */
  virtual bool setdeviceopt(int optcount, const void *optval, int optlen) = 0;
  virtual bool setdeviceopt_RWaveLead(int lead) = 0;
  virtual bool setdeviceopt_Delaytime(int delaytime) = 0;
  virtual bool setdeviceopt_RRInterval(float RRInterval) =0;
  /**
     * @param optval         option value
   * - std::string(or char*)
   * - int
   * - bool
   * - float
   * @param optlen         option length
   * - data type size
   * @return true if the Property is set successfully, otherwise false.
   * @see 
   */
  virtual bool getdeviceopt(int optname, const void *optval, int &optlen) = 0;

  // /**
  //  * @brief set RWave Preduction Lead  \n
  //  * @return result status 0：不启用 1：导联Ⅰ 2：导联Ⅱ 3：导联Ⅲ
  //  * @retval RESULT_OK       success
  //  * @retval RESULT_FAILE    failed  RWAVE_PREDUCTION_LEAD
  //  */
  // virtual result_t SetRWavePreductionLead(char status,uint32_t timeout = DEFAULT_TIMEOUT) = 0;

  //  /**
  //  * @brief set Configure Security Delay  \n
  //  * @return result tm   ms
  //  * @retval RESULT_OK       success
  //  * @retval RESULT_FAILE    failed  RWAVE_PREDUCTION_LEAD
  //  */
  // virtual result_t SetConfigureSecurityDelay(uint32_t tm,uint32_t timeout = DEFAULT_TIMEOUT) = 0;

  
  // /**
  //  * @brief Read Device Work Parameter \n
  //  * @param[in] info     Device Work Parameter
  //  * @param[in] timeout  timeout
  //  * @return result status
  //  * @retval RESULT_OK       success
  //  * @retval RESULT_FAILE or RESULT_TIMEOUT   failed
  //  */
  // virtual result_t ReadDeviceWorkParameter(WORK_PARAMETER &info,
  //                                uint32_t timeout = DEFAULT_TIMEOUT) = 0;

                                 
  /**
   * @brief Read Device Work Parameter \n
   * @param[in] info     Device Work Parameter
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE or RESULT_TIMEOUT   failed
   */
  virtual result_t getDeviceSHIPInfo(SHIP_INFO &info,
                                 uint32_t timeout = DEFAULT_TIMEOUT) = 0;

  // virtual int OperatorDevice(unsigned int cmd,const char * data,unsigned short dataSize) = 0;



    /**
   * @brief setDriverError
   * @param er
   */
  virtual void setDriverError(const DriverError &er) {
    ScopedLocker l(_error_lock);
    m_driverErrno = er;
  }
  
  /**
   * @brief getDriverError
   * @return
   */
  virtual DriverError getDriverError() {
    ScopedLocker l(_error_lock);
    return m_driverErrno;
  }
    virtual result_t SendTest(uint8_t cmd,uint8_t *test,int len) = 0;
  
// virtual std::map<std::string, std::string>  comPortList() ;
// {

// }
unsigned short modbusCRC16(unsigned char *puchMsg, unsigned int len)
{

    unsigned char uchCRCHi=0xff;
    unsigned char uchCRCLo=0xff;
    unsigned int  uindex;
    while(len--)
    {
        uindex=uchCRCHi^*puchMsg++;
        uchCRCHi=uchCRCLo^auchCRCHi[uindex];
        uchCRCLo=auchCRCLo[uindex];
    }
    return (uchCRCHi<<8|uchCRCLo);
}
protected:
  enum {
    DATA_TIMEOUT=100,
    DEFAULT_TIMEOUT = 200,    /**< Default timeout. */
    DEFAULT_HEART_BEAT = 1000, /**< Default heartbeat timeout. */
    MAX_DATA_NODES = 7200,	   /**< Default Max Scan Count. */
    DEFAULT_TIMEOUT_COUNT = 1, /**< Default Timeout Count. */
  };
protected:
    MsgCallback *m_callback;
    void* m_userId = 0;
    DEVICMODE  m_deviceMode;
    DEVICETYPE m_DeviceType;
    long m_DeviceId = 0;

protected:
  bool            m_isScanning;
  /// device connected state
  bool            m_isConnected;
  /// Scan Data Event
  Event           _dataEvent;
  /// Data Locker
  Locker          _lock;
  /// Parse Data thread
  Thread          _thread;
  /// command locker
  Locker          _cmd_lock;
  /// driver error locker
  Locker          _error_lock;

  ///com port or IP Address
  std::string serial_port;
  /// baudrate or IP port
  uint32_t m_baudrate;

  
  /// number of last error
  DriverError m_driverErrno;

  ///
  int retryCount;
    /// auto reconnect
  bool isAutoReconnect;
  /// auto connecting state
  bool isAutoconnting;
  size_t m_BufferSize;

};

}//common
}//core
}//accu