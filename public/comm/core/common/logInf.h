#pragma once
#include <string>
#include <iostream>
#include <fstream>
#if defined(_WIN32)
#include <io.h>
#include <direct.h>
#include <windows.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>
#endif
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include <sstream>
#include <stdarg.h>

#define gettid() syscall(__NR_gettid)
#define MAX_LEN 1024


namespace accu {
namespace core {
namespace common {
static std::mutex logMutex;
enum LogLevelEnum
{
	INFO = 0,
	DEBUG = 1,
	WARN = 2,
	EERROR = 3,
	CRITICAL = 4
};

class logInf
{
public:
    
    logInf();
	logInf(std::string logDir, std::string prjname);
	virtual ~logInf();
public:
	static logInf* getInstance();
	static time_t getTimeStamp();
	static std::tm* gettm(uint64_t timestamp);
	static std::string getTimeStr();
	static std::string getFilePath();
public:
	void InitialStrLog(std::string logDir, std::string prjname);
	void LogString(std::string strInf, LogLevelEnum nLevel, std::string filename, int nrow);
	void LogString(LogLevelEnum nLevel, std::string filename, int nrow,const char* format, ...);
	void LogString2(LogLevelEnum nLevel,const char* format, ...);
    
private:
    std::string strlogPath;
	static logInf* instance;
};

}
}
}