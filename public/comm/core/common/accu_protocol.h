/*********************************************************************
*********************************************************************/
#pragma once
#include <core/base/v8stdint.h>
#include <vector>
#include <functional>

/// Count the number of elements in a statically allocated array.
#if !defined(_countof)
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

/**
 * @name PI constant
 * @{
 */
#ifndef M_PI
#define M_PI 3.1415926
#endif
/** @}
*/




/**
 * @name Protocol head flag 
 * @{
 */
#define PROTOCOL_HEADER_FLAG                      0x55


/**@name ACCU CMD Protocol
* @brief ACCU request and response CMD
* @{
*/
#define ACCU_CMD_READ_REGISTER_DATA_START               0x02 ///< 
#define ACCU_CMD_IMP_SWITCH                             0x03 ///<  SURVEY_ADAS1000 
#define ACCU_CMD_WAVE_FILTER_DATA                       0x04 ///<  SURVEY_ADAS1000  
#define ACCU_CMD_ECG_START                              0x06 ///<  SURVEY_ADAS1000
#define ACCU_CMD_ECG_DATA_GET                           0x07
#define ACCU_CMD_ECG_STATUS_GET                         0x0C

#define ACCU_CMD_RESPIRATION_DATA_GET                   0x08
#define ACCU_CMD_PACEMAKE_DATA_GET                      0x09
#define ACCU_CMD_DEBUGPRINT_DATA_GET                    0x0A
#define ACCU_CMD_RPLOT_DATA_GET                         0x0B

#define ACCU_CMD_IMP_START                              0x20 ///<  SURVEY_AD5940
#define ACCU_CMD_IMP_DATA_GET                           0x21
#define ACCU_CMD_IMP_STATUS_GET                         0x22


#define ACCU_CMD_CHIP_STATUS_GET                        0x0D
#define ACCU_CMD_WORK_PARAMETER_DATA_SET                0x0E
#define ACCU_CMD_WORK_PARAMETER_DATA_GET                0x10
#define ACCU_CMD_PRODUCT_INFORMATION_DATA_GET           0x11
#define ACCU_CMD_PRODUCT_INFORMATION_DATA_SET           0x12



/// Maximuum number of samples in a packet
#define PackageSampleMaxLngth 0x100


#if defined(_WIN32)
#pragma pack(1)
#endif




///  ADAS1000 Status
struct DEVICE_ECG_STATUS 
 {
  uint8_t  STATUS; ///<
  uint8_t  ADC_STATUS;
  uint8_t  DCLO_HI_STATUS;
  uint8_t  DCLO_LO_STATUS;

} __attribute__((packed)) ;

struct DEVICE_IMP_STATUS 
 {
  uint8_t  ADC_Value; ///<
  uint8_t  ADC_Sequencer;
  uint8_t  Data_Fifo;
  uint8_t  Outlier;

} __attribute__((packed)) ;


///  ADAS1000 Status
// struct DEVICE_IMP_STATUS
//  {
//   uint32_t   workstatus; ///< 
// } __attribute__((packed)) ;

///  DEVICE_WORK_PARAMETER Information
struct DEVICE_WORK_PARAMETER
 {
  uint8_t   R_Lead_type; ///<  
  uint32_t  deley_time; ///<

} __attribute__((packed)) ;

/// device Health Information
struct device_health {
  uint8_t   status; ///< health state
  uint16_t  error_code; ///< error code
} __attribute__((packed))  ;



/// device response Header
struct device_ans_header {
  uint8_t syncByte;
  uint8_t dvrdressByte;
  uint8_t cmd_flag;
  uint16_t size;
} __attribute__((packed));

/// device request command packet
struct cmd_packet {
  device_ans_header header;
  uint8_t data[255];
  uint16_t checksum;
} __attribute__((packed)) ;

/// device response body
struct device_ans_response {
  uint8_t responseByte;
} __attribute__((packed));

/// device response body
struct device_ans_SHIP_response {
  uint8_t ecg_work_status;
  uint8_t ecg_status;
  uint8_t  ADC_STATUS;
  uint8_t  DCLO_HI_STATUS;
  uint8_t  DCLO_LO_STATUS;
  uint8_t  IMP_Work_status;
  uint32_t IMP_status;
  
} __attribute__((packed));


#if defined(_WIN32)
#pragma pack()
#endif

/// Package Header Size
#define PackagePaidBytes sizeof(device_ans_header)


#include "accu_datatype.h"
