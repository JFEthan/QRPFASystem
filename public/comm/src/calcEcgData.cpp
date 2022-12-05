#include "calcEcgData.h"
#include <core/base/thread.h>

namespace accu {
using namespace core::base;

calcEcgData::calcEcgData()
{
    CLASS_THREAD(calcEcgData, calcECGDataThread);
    resetCalc();
}
calcEcgData::~calcEcgData()
{
    isExistThread = true;
}
void calcEcgData::resetCalc()
{
    {
        lock_guard_t mutex_guard_lock(m_signalque_mutex);
        m_signalque.clear();
    }
    {
        lock_guard_t mutex_guard_lock(m_filterSignaldeque_mutex); 
        m_filterSignaldeque.clear();
    }
    
    m_index = 0;
    avegflag = false;
    avegVlue = 0.0;
    m_calcIndex = 0;
    isFirst = true;
}
 int calcEcgData::calcECGDataThread() 
 {
     int beginPos = 143;
     while(!isExistThread)
     {
         std::deque<ECGData> signalqueTemp;
         int nCount = 0;
         
        {
            lock_guard_t mutex_guard_lock(m_signalque_mutex);
            signalqueTemp = m_signalque;
            nCount = m_signalque.size();
        }
        if(nCount == 0)
          delay(200);
        if(nCount <  FILTER_COUNTS)
          delay(10);  
        if(nCount >=  FILTER_COUNTS)
        {
            double b_signal[FILTER_COUNTS] = {0};
            for(int i=0;i<FILTER_COUNTS;i++)
            {
                b_signal[i] = signalqueTemp[i].ecgValue- avegVlue;
            }

            double filterSignal[FILTER_COUNTS] = {0};
            windowsKaiser285(b_signal, filterSignal);
            //windowsKaiser(b_signal,filterSignal);
            double *pos = filterSignal ;
            
            pos += 400;       
            for (int i = 0; i < FILTER_WIN; i++)
            {
                ECGData ecgDataTemp = signalqueTemp.at(257+i);
                ecgDataTemp.ecgValue = (float)pos[0]; pos++;
                {
                    lock_guard_t mutex_guard_lock(m_filterSignaldeque_mutex); 
                    m_filterSignaldeque.push_back(ecgDataTemp);
                }
            
            }
            

            {
                lock_guard_t mutex_guard_lock(m_signalque_mutex);
                if(m_signalque.size() >= FILTER_WIN)
                {
                    m_signalque.erase(m_signalque.begin(), m_signalque.begin()+FILTER_WIN);//{}，
                }
            }

            
        }
     }
     return 1;
 }
//void OutputDebugPrintf(const char* strOutputString, ...)
//{
//	char strBuffer[4096] = { 0 };
//
//	va_list vlArgs;
//	va_start(vlArgs, strOutputString);
//	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
//
//	va_end(vlArgs);
//	OutputDebugString(strBuffer);
//}
std::tuple<int, std::vector<ECGData>>  calcEcgData::addEcgData(ECGData ecgData)
{
    std::vector<ECGData> filterSignalVec ;
    std::deque<ECGData>TempfilterSignaldeque;
    int nResult = -1;
    

    {
        lock_guard_t mutex_guard_lock(m_signalque_mutex);
        m_signalque.push_back(ecgData);
    }

    {
        lock_guard_t mutex_guard_lock(m_filterSignaldeque_mutex);
        TempfilterSignaldeque = m_filterSignaldeque;
    }

    if(TempfilterSignaldeque.size() >= FILTER_WIN)
    {
        for (int i = 0; i < FILTER_WIN; i++)
        {
            ECGData temp = TempfilterSignaldeque.at(i);
            filterSignalVec.push_back(temp);
        }
        {
            lock_guard_t mutex_guard_lock(m_filterSignaldeque_mutex);
             if(m_filterSignaldeque.size() >= FILTER_WIN)
             {
                 m_filterSignaldeque.erase(m_filterSignaldeque.begin(), m_filterSignaldeque.begin()+FILTER_WIN);//{}，
             }
           
        }
        
        nResult = 1;
    }

    
   
    return std::make_tuple(nResult, filterSignalVec);

}
bool calcEcgData::init420()
{
    Fir420Init();
    notchInit();
    return true;
}

std::tuple<int, std::vector<ECGData>>   calcEcgData::addEcgDataTo420(ECGData &ecgData)
{
    std::vector<ECGData> filterSignalVec ;
   ecgData.ecgValue = notchProc(Fir420FilterExt(ecgData.ecgValue)/1000.0); //单位uv，需要转mv
   ecgData.serialNo -= 210;
   if(ecgData.serialNo >= 0)
   {
    filterSignalVec.push_back(ecgData);
    return std::make_tuple(1, filterSignalVec);
   }
   

   return std::make_tuple(0, filterSignalVec);
}

// bool calcEcgData::checkCalc()
// {
//     bool bcalcFlag = false;
//     if (m_index < FILTER_COUNTS)
//     {
//         bcalcFlag = false;
//         m_calcIndex = 0;
//     }
//     else if ((m_index-FILTER_COUNTS)%FILTER_WIN == 0)
//     {
//         bcalcFlag = true;
//         m_calcIndex = m_index;
//     }
//     return bcalcFlag;
// }
// bool calcEcgData::findEcgData(ECGData ecgData)
// {
//     bool bres = false;
//     std::map<int,ECGData>::iterator it = m_ecgDisplay.find(ecgData.serialNo);
//     if(it == m_ecgDisplay.end())
//     {
//         m_ecgDisplay.insert(std::pair<int,ECGData>(ecgData.serialNo,ecgData));
//     }
//     else{
//         bres = true;
//     }
//     if(m_ecgDisplay.size() > 20000)
//     {
// 		std::map<int, ECGData>::iterator it2 = m_ecgDisplay.begin();
//         m_ecgDisplay.erase(it2);
//     }
//     return bres;
// }

}