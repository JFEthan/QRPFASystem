#ifndef comm_def_h
#define comm_def_h
#include <stdint.h>
#include <stdlib.h>
#include <core/base/v8stdint.h>
#include <core/base/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif


#pragma pack(1)

enum DEVICETYPE {
  DEVICE_TYPE_RF = 1
};

enum DEVICMODE {
  DEVICE_MODE_SERIAL = 0,
  DEVICE_MODE_TCP = 1,
};

enum OPERATOR_DEVICE_CMD {
  OPT_SURVEYADAS1000 =0,
  OPT_SURVEYAD5940 = 1
};

///  DEVICE_WORK_PARAMETER Information
typedef struct 
 {
  uint8_t  parameter_type; ///<  
  uint8_t  parameter_data[4]; ///<
} WORK_PARAMETER;

struct SHIP_INFO {
  uint8_t ecg_work_status;
  uint8_t ecg_status;
  uint8_t  ADC_STATUS;
  uint8_t  DCLO_HI_STATUS;
  uint8_t  DCLO_LO_STATUS;
  uint8_t  IMP_Work_status;
  uint32_t IMP_status;  
} __attribute__((packed));

/**
  * @brief c string
  */
typedef struct {
  /// data
  char data[50];
} string_t;

/**
  * @brief accu ports
  */
typedef struct {
  string_t port[8];
} AccuPort;

/** The numeric version information struct.  */
typedef struct {
  uint8_t hardware;   /**< Hardware version*/
  uint8_t soft_major;      /**< major number */
  uint8_t soft_minor;      /**< minor number */
  uint8_t soft_patch;      /**< patch number */
  uint8_t sn[16];     /**< serial number*/
} AccuVersion;

/// accu instance
typedef struct {
  void *lpDevice;///< CDevice instance
} accuDevice;

typedef enum  {
  NoError = 0,
  DeviceNotFoundError,
  PermissionError,
  UnsupportedOperationError,
  UnknownError,
  TimeoutError,
  NotOpenError,
  BlockError,
  NotBufferError,
  TrembleError,
} DriverError;

typedef struct {
  char channel;  
  short frame;
  int serialNo;
  float ecgValue; 
} ECGData;

typedef struct {
  char channel;   
  float impValue; 
} IMPData;

typedef struct {
  char channel;   
  short frame;//0:600 1:500
  int R_index;
  int RPlot_position;
  float RPlot_position_value;
  float rrInterval;
  float rWaveDelayCount;
  int ecg_position;
  float ecg_position_Value;
  float standard_deviation;

} RPlotData;

typedef struct {
  DriverError messageType;
  char mesageName[255];
  char mesagedes[255];
} RMESSAGEData;

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif