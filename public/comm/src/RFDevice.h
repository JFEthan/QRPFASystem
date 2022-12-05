#ifndef RFDevice_h
#define RFDevice_h
#include <core/common/ChannelDevice.h>
#include <core/common/IDevice.h>
#include <core/common/accu_protocol.h>
#include <queue>
#include <mutex>
#include <map>
#include <string>
#include "calcEcgData.h"
// #include <core/base/locker.h>
// #include <core/base/thread.h>

namespace accu {
using namespace core;
using namespace core::common;

class RFDevice : public IDevice
{
public:   
    RFDevice(DEVICMODE device_mode);
    ~RFDevice();

public:

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
  virtual result_t connectDevice(const char *port_path, uint32_t baudrate = 8000) ;
    /*!
  * @brief Disconnect the LiDAR.
  */
  virtual void disconnect() ;

    /**
   * @brief Is it connected to the device \n
   * @return connection status
   * @retval true     connected
   * @retval false    Non-connected
   */
  virtual bool isconnected() const ;

  /**
   * @brief Is the device in the scan \n
   * @return scanning status
   * @retval true     scanning
   * @retval false    non-scanning
   */
  virtual bool isscanning() const;

  /**
   * @brief Turn on ECG \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
  virtual result_t startECG(uint32_t timeout = DEFAULT_TIMEOUT) ;

  /**
   * @brief turn off ECG \n
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
  virtual result_t stopECG(uint32_t timeout = DEFAULT_TIMEOUT) ;

     /**
   * @brief Turn on IMP \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
  virtual result_t resetIMP(uint32_t timeout = DEFAULT_TIMEOUT) ;
     /**
   * @brief Turn on IMP \n
   * @param[in] force    Scan mode
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   * @note Just turn it on once
   */
  virtual result_t startIMP(uint32_t timeout = DEFAULT_TIMEOUT) ;
 /**
   * @brief turn off IMP \n
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
  virtual result_t stopIMP(uint32_t timeout = DEFAULT_TIMEOUT) ;
 
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
  virtual bool setdeviceopt(int optcount, const void *optval, int optlen);
  virtual bool setdeviceopt_RWaveLead(int lead) ;
  virtual bool setdeviceopt_Delaytime(int delaytime) ;
  virtual bool setdeviceopt_RRInterval(float RRInterval) ;
  
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
  virtual bool getdeviceopt(int optname, const void *optval, int &optlen);

  /**
   * @brief get ship status information \n
   * @param[in] info     Device information
   * @param[in] timeout  timeout
   * @return result status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE or RESULT_TIMEOUT   failed
   */
 virtual result_t getDeviceSHIPInfo(SHIP_INFO &info,
                                 uint32_t timeout = DEFAULT_TIMEOUT);


virtual result_t SendTest(uint8_t cmd,uint8_t *test,int len) ;
 
protected:
  // int optsurveyADAS1000(const char * data,unsigned int dataSize);

 protected:
 
  result_t stopRecv(uint32_t timeout) ;

  /*!
   * @brief checkAutoConnecting
   */
  result_t checkAutoConnecting(bool serialError = true);
   /**
  * @brief data parsing thread \n
  */
  int cacheRecvData();
  int disposeECGData();
  int disposeIMPData();
  int disposeNomalData();
  int disposeRpotData(cmd_packet cmd_packet_);
  int disposeECGStatusData(cmd_packet cmd_packet_);
  int disposeChipData(cmd_packet cmd_packet_);
   /**
  * @brief Data parsing thread \n
  * @note Before you create a dta parsing thread, you must use the ::startScan function to start the lidar scan successfully.
  */
  result_t createThread();
   /**
   * @brief waiting device information
   * @param timeout timeout
   * @return status
   * @retval RESULT_OK       success
   * @retval RESULT_FAILE    failed
   */
  result_t waitDevicePackage(uint32_t timeout = DEFAULT_TIMEOUT);

  /**
  * @brief waiting for package header \n
  * @param[in] header 	 package header
  * @param[in] timeout      timeout
  * @return return status
  * @retval RESULT_OK       success
  * @retval RESULT_TIMEOUT  timeout
  * @retval RESULT_FAILE    failed
  * @note when timeout = -1, it will block...
  */
  result_t waitRecvHeader(device_ans_header *header,
                              uint32_t timeout = DEFAULT_TIMEOUT);
    /**
  * @brief Waiting for the specified size data from the lidar \n
  * @param[in] data_count 	 wait max data size
  * @param[in] timeout    	 timeout
  * @param[in] returned_size   really data size
  * @return return status
  * @retval RESULT_OK       success
  * @retval RESULT_TIMEOUT  wait timeout
  * @retval RESULT_FAILE    failed
  * @note when timeout = -1, it will block...
  */
  result_t waitForData(size_t data_count, uint32_t timeout = DEFAULT_TIMEOUT,
                       size_t *returned_size = NULL);

  /**
  * @brief send data to lidar \n
  * @param[in] cmd 	 command code
  * @param[in] payload      payload
  * @param[in] payloadsize      payloadsize
  * @return result status
  * @retval RESULT_OK       success
  * @retval RESULT_FAILE    failed
  */
  result_t sendCommand(uint8_t cmd, const void *payload = NULL,
                       size_t payloadsize = 0);

    /**
  * @brief send data to serial \n
  * @param[in] data 	 data
  * @param[in] size    data size
  * @return return status
  * @retval RESULT_OK       success
  * @retval RESULT_FAILE    failed
  */
  result_t sendData(const uint8_t *data, size_t size);

    /**
  * @brief get data from serial \n
  * @param[in] data 	 data
  * @param[in] size    date size
  * @return return status
  * @retval RESULT_OK       success
  * @retval RESULT_FAILE    failed
  */
  result_t getData(uint8_t *data, size_t size);


    /*!
   * @brief flushSerial
   */
  void flushSerial();

  float switchValue(float value);
  calcEcgData* GetCalEcg(uint8_t nchannle) ;

private: 
  bool checkPackageLength( uint8_t cmd_flag, uint16_t size);
  std::string HexToString(std::uint8_t *in, int len);

private:
  // std::string m_SerialPort;
  // int m_baudrate;
  bool m_isConnected = false;
  ChannelDevice *_serial = nullptr;

  bool has_device_header = false;
  uint8_t last_device_byte;
  int         asyncRecvPos;
  uint16_t    async_size;

  uint8_t m_RecvBuffer[1024*10];
  uint32_t m_RecvBufferSize = 0;
  uint32_t m_heartbeat_ts = 0;
  //device_ans_header packet_header;
  std::queue<cmd_packet> nomal_infos;
  Locker          _nomal_data_lock;
   std::queue<cmd_packet> ecg_infos;
  Locker          _ecg_data_lock;
   std::queue<cmd_packet> imp_infos;
  Locker          _imp_data_lock;
  bool is_ecg_dispose_thread = false;
  bool is_imp_dispose_thread = false;
  bool is_nomal_dispose_thread = false;
  //std::vector<ECGData> vectorEcgData;
  Locker          _mapEcgData_lock;
  std::map<uint8_t,calcEcgData*> mapEcgData;
  // bool isCalAveragedValue = false;
  // float AveragedValue = 0.00;
  // std::vector<ECGData> tempEcgData;
  int m_nChannel = 0;
  HANDLE hEvent = NULL;
  Thread          _thread1 ;
  Thread          _thread2 ;
  Thread          _thread3 ;
};
}

#endif // RFDevice_h