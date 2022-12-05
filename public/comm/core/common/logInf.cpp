#include "logInf.h"


namespace accu {
namespace core {
namespace common {

  logInf* logInf::instance = NULL;  

 logInf::logInf()
 {

 }

logInf::logInf(std::string logDir, std::string prjname)
{
    InitialStrLog(logDir, prjname);
}

logInf::~logInf()
{
    
}
logInf* logInf::getInstance()
{
    if(logInf::instance == NULL) 
    {
        logInf::instance = new logInf();
        time_t systime = logInf::getTimeStamp();
        std::stringstream ss;
        ss<<systime; 
        std::string  filenameTime=ss.str();

        std::string filename = "accu_comm_sdk_"+filenameTime+".log";
        instance->InitialStrLog(logInf::getFilePath(),filename);
    }
		
	return logInf::instance;

}
time_t logInf::getTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    time_t timestamp = tmp.count();
    return timestamp;
}
std::tm* logInf::gettm(uint64_t timestamp)
{
    uint64_t milli = timestamp;
    milli += (uint64_t)8*60*60*1000;//转换时区，北京时间+8小时
    auto mTime = std::chrono::milliseconds(milli);
    auto tp=std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds>(mTime);
    auto tt = std::chrono::system_clock::to_time_t(tp);
    std::tm* now = gmtime(&tt);
    return now;
}
    
std::string logInf::getTimeStr()
{
    time_t timep;
    timep = getTimeStamp(); 
    struct tm *info;
    info = gettm(timep); 
    
    char tmp[1000] = {0};
    sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d.%03ld", info->tm_year+1900, info->tm_mon+1, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec, timep%1000000);
    return tmp;
}
std::string logInf::getFilePath()
{
   char cwd[256];
    _getcwd(cwd, 256);
    std::cout << cwd << std::endl;
    std::string path = cwd;
    std::string &tempPath = path;
    std::string::size_type p=tempPath.find("\\");
    while (p != std::string::npos) {
        p= p = tempPath.find("\\");
        if (p!=std::string::npos)
        {
            tempPath.replace(p, 1, "/");
        }
    }
    return tempPath;
}


void logInf::InitialStrLog(std::string logDir, std::string prjname)
{
    strlogPath = logDir + "/" + prjname;
#ifdef WIN32
    int iRet = _access(strlogPath.c_str(), 0);
#else
    int iRet = access(strlogPath.c_str(), 0);
#endif
    if (iRet != 0)
    {
        std::ofstream ofs(strlogPath, std::ios::app);
        if (ofs.is_open())
        {
            std::cout << "file is open !" << std::endl;
            ofs.close();
        }
    }
}



void logInf::LogString(std::string strInf, LogLevelEnum nLevel, std::string filename, int nrow)
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, 23, "%Y-%m-%d %H:%m:%s", localtime(&timep));
    std::string strTime = getTimeStr(); //tmp;
    std::string strTimeOut = "{" + strTime + "}";

    std::string strloglevel;
    if (nLevel == LogLevelEnum::INFO)
    {
        strloglevel = "{INFO}";
    }
    else if (nLevel == LogLevelEnum::DEBUG)
    {
        strloglevel = "{DEBUG}";
    }
    else if (nLevel == LogLevelEnum::WARN)
    {
        strloglevel = "{WARN}";
    }
    else if (nLevel == LogLevelEnum::EERROR)
    {
        strloglevel = "{ERROR}";
    }
    else if (nLevel == LogLevelEnum::CRITICAL)
    {
        strloglevel = "{CRITICAL}";
    }
    int findex = filename.find_last_of("/");
    if(findex < 0) findex = 0;
    std::string strfilename = filename.substr(findex+1);
    std::string strFuncName = "{" + strfilename + "}";
    
    int lineRow = __LINE__;
    std::stringstream ss;
    ss << lineRow;
    std::string strRowNum = "{" + ss.str() + "}";

    std::stringstream sthread;
    int nTreadID;
#ifdef WIN32
    nTreadID = GetCurrentThreadId();
#else
    nTreadID = gettid();
#endif
    sthread << nTreadID;
    std::string strTreadID = "{" + sthread.str() + "}";

    logMutex.lock();
    std::string strlineInf = strTimeOut + strloglevel + strFuncName + strRowNum + strTreadID + ": " + strInf;
    std::ofstream ofs(strlogPath, std::ios::app);
    if (ofs.is_open())
    {
        ofs << strlineInf << std::endl;
        std::cout<<strlineInf<<std::endl;
    }
    logMutex.unlock();
}



void logInf::LogString(LogLevelEnum nLevel, std::string filename, int nrow,const char* format, ...)
{
    char message[MAX_LEN];
    va_list args;
    va_start(args, format);
    vsnprintf (message, MAX_LEN, format, args);
    va_end(args);

    time_t timep= time(NULL);
    char tmp[64];
    strftime(tmp, 23, "%Y-%m-%d %H:%M:%S", localtime(&timep));
    std::string strTime = getTimeStr(); //tmp;
    std::string strTimeOut = "{" + strTime + "}";

    std::string strloglevel;
    if (nLevel == LogLevelEnum::INFO)
    {
        strloglevel = "{INFO}";
    }
    else if (nLevel == LogLevelEnum::DEBUG)
    {
        strloglevel = "{DEBUG}";
    }
    else if (nLevel == LogLevelEnum::WARN)
    {
        strloglevel = "{WARN}";
    }
    else if (nLevel == LogLevelEnum::EERROR)
    {
        strloglevel = "{ERROR}";
    }
    else if (nLevel == LogLevelEnum::CRITICAL)
    {
        strloglevel = "{CRITICAL}";
    }
    int findex = filename.find_last_of("/");
    if(findex < 0) findex = 0;
    std::string strfilename = filename.substr(findex+1);
    std::string strFuncName = "{" + strfilename + "}";
    
    int lineRow = nrow;
    std::stringstream ss;
    ss << lineRow;
    std::string strRowNum = "{" + ss.str() + "}";

    std::stringstream sthread;
    int nTreadID;
#ifdef WIN32
    nTreadID = GetCurrentThreadId();
#else
    nTreadID = gettid();
#endif
    sthread << nTreadID;
    std::string strTreadID = "{" + sthread.str() + "}";

    logMutex.lock();
    std::string strInf = message;
    std::string strlineInf = strTimeOut + strloglevel + strFuncName + strRowNum + strTreadID + ": " + strInf;
    std::ofstream ofs(strlogPath, std::ios::app);
    if (ofs.is_open())
    {
        ofs << strlineInf << std::endl;
        std::cout<<strlineInf<<std::endl;
    }
    logMutex.unlock();
}



void logInf::LogString2(LogLevelEnum nLevel,const char* format, ...)
{
    char message[MAX_LEN];
    va_list args;
    va_start(args, format);
    vsnprintf (message, MAX_LEN, format, args);
    va_end(args);

    time_t timep= time(NULL);
    char tmp[64];
    strftime(tmp, 23, "%Y-%m-%d %H:%M:%S", localtime(&timep));
    std::string strTime = getTimeStr(); //tmp;
    std::string strTimeOut = "{" + strTime + "}";

    std::string strloglevel;
    if (nLevel == LogLevelEnum::INFO)
    {
        strloglevel = "{INFO}";
    }
    else if (nLevel == LogLevelEnum::DEBUG)
    {
        strloglevel = "{DEBUG}";
    }
    else if (nLevel == LogLevelEnum::WARN)
    {
        strloglevel = "{WARN}";
    }
    else if (nLevel == LogLevelEnum::EERROR)
    {
        strloglevel = "{ERROR}";
    }
    else if (nLevel == LogLevelEnum::CRITICAL)
    {
        strloglevel = "{CRITICAL}";
    }
   
    std::stringstream sthread;
    int nTreadID;
#ifdef WIN32
    nTreadID = GetCurrentThreadId();
#else
    nTreadID = gettid();
#endif
    sthread << nTreadID;
    std::string strTreadID = "{" + sthread.str() + "}";

    logMutex.lock();
    std::string strInf = message;
    std::string strlineInf = strTimeOut + strloglevel +  strTreadID + ": " + strInf;
    std::ofstream ofs(strlogPath, std::ios::app);
    if (ofs.is_open())
    {
        ofs << strlineInf << std::endl;
        std::cout<<strlineInf<<std::endl;
    }
    logMutex.unlock();
}

}
}
}
