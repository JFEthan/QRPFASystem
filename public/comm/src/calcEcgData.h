#ifndef CALECGDATA_H
#define CALECGDATA_H

#include <core/common/IDevice.h>
#include <core/common/accu_protocol.h>
#include <deque>
#include <mutex>
#include <map>
#include "calcEcgData.h"
#include <core/FIR-Matlab/windowsKaiser.h>
#include <atomic>

namespace accu {

#define MAX_FILTER_BUFF  50000
#define FILTER_COUNTS FILTER_COUNTS_F
#define FILTER_WIN    FILTER_WIN_F
#define FILTER_GRPLEN FILTER_GRPLEN_F/2


class calcEcgData
{
public:   
    calcEcgData();
    ~calcEcgData();
public:
   void resetCalc();
   std::tuple<int, std::vector<ECGData>>  addEcgData(ECGData ecgData);
   
   bool init420();
   std::tuple<int, std::vector<ECGData>>   addEcgDataTo420(ECGData &ecgData);
   
protected:   
   bool checkCalc();
   int calcECGDataThread() ;
   bool findEcgData(ECGData ecgData);
protected: 
    bool isExistThread = false;
    bool avegflag = false;
    double avegVlue = 0.0;
    int m_index = 0;
    int m_calcIndex = 0;
    // std::vector<ECGData> m_signalVec;
    
    using lock_guard_t = std::lock_guard<std::mutex>;
    std::mutex m_signalque_mutex;
    std::deque<ECGData> m_signalque;
    
    std::mutex m_ecgDisplay_mutex;
    std::map<int,ECGData> m_ecgDisplay;

    std::mutex m_filterSignaldeque_mutex;
    std::deque<ECGData>m_filterSignaldeque;
    std::atomic<bool>  isFirst = true;

};
}
#endif // CALECGDATA_H