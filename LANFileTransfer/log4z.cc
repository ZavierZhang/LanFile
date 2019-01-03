
#include "log4z.h"


#ifdef WIN32
#include <io.h>
#include <shlwapi.h>
#include <process.h>
#pragma comment(lib, "shlwapi")
#pragma warning(disable:4996)

#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <semaphore.h>
#endif


#ifdef __APPLE__
#include <dispatch/dispatch.h>
#include <libproc.h>
#endif


_LOG4Z_BEGIN

//////////////////////////////////////////////////////////////////////////
//! Log4zFileHandler
//////////////////////////////////////////////////////////////////////////

const std::string Log4zFileHandler::readContent()
{
    std::string content;
    
    if (!_file)
    {
        return content;
    }
    fseek(_file, 0, SEEK_SET);
    int beginpos = ftell(_file);
    fseek(_file, 0, SEEK_END);
    int endpos = ftell(_file);
    fseek(_file, 0, SEEK_SET);
    int filelen = endpos - beginpos;
    if (filelen > 10*1024*1024 || filelen <= 0)
    {
        return content;
    }
    content.resize(filelen+10);
    if (fread(&content[0], 1, filelen, _file) != (size_t)filelen)
    {
        content.clear();
        return content;
    }
    content = content.c_str();
    return content;
}




//////////////////////////////////////////////////////////////////////////
//! utility
//////////////////////////////////////////////////////////////////////////


void sleepMillisecond(unsigned int ms)
{
#ifdef WIN32
    ::Sleep(ms);
#else
    usleep(1000*ms);
#endif
}

struct tm timeToTm(time_t t)
{
#ifdef WIN32
#if _MSC_VER < 1400 //VS2003
    return * localtime(&t);
#else //vs2005->vs2013->
    struct tm tt = { 0 };
    localtime_s(&tt, &t);
    return tt;
#endif
#else //linux
    struct tm tt = { 0 };
    localtime_r(&t, &tt);
    return tt;
#endif
}

bool isSameDay(time_t t1, time_t t2)
{
    tm tm1 = timeToTm(t1);
    tm tm2 = timeToTm(t2);
    if ( tm1.tm_year == tm2.tm_year
        && tm1.tm_yday == tm2.tm_yday)
    {
        return true;
    }
    return false;
}


void fixPath(std::string &path)
{
    if (path.empty()){return;}
    for (std::string::iterator iter = path.begin(); iter != path.end(); ++iter)
    {
        if (*iter == '\\'){*iter = '/';}
    }
    if (path.at(path.length()-1) != '/'){path.append("/");}
}

static void trimLogConfig(std::string &str, std::string extIgnore)
{
    if (str.empty()){return;}
    extIgnore += "\r\n\t ";
    int length = (int)str.length();
    int posBegin = 0;
    int posEnd = 0;
    
    //trim utf8 file bom
    if (str.length() >= 3
        && (unsigned char)str[0] == 0xef
        && (unsigned char)str[1] == 0xbb
        && (unsigned char)str[2] == 0xbf)
    {
        posBegin = 3;
    }
    
    //trim character
    for (int i = posBegin; i<length; i++)
    {
        bool bCheck = false;
        for (int j = 0; j < (int)extIgnore.length(); j++)
        {
            if (str[i] == extIgnore[j])
            {
                bCheck = true;
            }
        }
        if (bCheck)
        {
            if (i == posBegin)
            {
                posBegin++;
            }
        }
        else
        {
            posEnd = i + 1;
        }
    }
    if (posBegin < posEnd)
    {
        str = str.substr(posBegin, posEnd-posBegin);
    }
    else
    {
        str.clear();
    }
}

//split
static std::pair<std::string, std::string> splitPairString(const std::string & str, const std::string & delimiter)
{
    std::string::size_type pos = str.find(delimiter.c_str());
    if (pos == std::string::npos)
    {
        return std::make_pair(str, "");
    }
    return std::make_pair(str.substr(0, pos), str.substr(pos+delimiter.length()));
}

static bool parseConfigLine(const std::string& line, int curLineNum, std::string & key, std::map<std::string, LoggerInfo> & outInfo)
{
    std::pair<std::string, std::string> kv = splitPairString(line, "=");
    if (kv.first.empty())
    {
        return false;
    }
    
    trimLogConfig(kv.first);
    trimLogConfig(kv.second);
    if (kv.first.empty() || kv.first.at(0) == '#')
    {
        return true;
    }
    
    if (kv.first.at(0) == '[')
    {
        trimLogConfig(kv.first, "[]");
        key = kv.first;
        {
            std::string tmpstr = kv.first;
            std::transform(tmpstr.begin(), tmpstr.end(), tmpstr.begin(), ::tolower);
            if (tmpstr == "main")
            {
                key = "Main";
            }
        }
        std::map<std::string, LoggerInfo>::iterator iter = outInfo.find(key);
        if (iter == outInfo.end())
        {
            LoggerInfo li;
            li._enable = true;
            li._key = key;
            li._name = key;
            outInfo.insert(std::make_pair(li._key, li));
        }
        else
        {
            std::cout << "log4z configure warning: duplicate logger key:["<< key << "] at line:" << curLineNum << std::endl;
        }
        return true;
    }
    trimLogConfig(kv.first);
    trimLogConfig(kv.second);
    std::map<std::string, LoggerInfo>::iterator iter = outInfo.find(key);
    if (iter == outInfo.end())
    {
        std::cout << "log4z configure warning: not found current logger name:["<< key << "] at line:" << curLineNum
        << ", key=" << kv.first << ", value=" << kv.second << std::endl;
        return true;
    }
    std::transform(kv.first.begin(), kv.first.end(), kv.first.begin(), ::tolower);
    //! path
    if (kv.first == "path")
    {
        iter->second._path = kv.second;
        return true;
    }
    else if (kv.first == "name")
    {
        iter->second._name = kv.second;
        return true;
    }
    std::transform(kv.second.begin(), kv.second.end(), kv.second.begin(), ::tolower);
    //! level
    if (kv.first == "level")
    {
        if (kv.second == "trace" || kv.second == "all")
        {
            iter->second._level = LOG_LEVEL_TRACE;
        }
        else if (kv.second == "debug")
        {
            iter->second._level = LOG_LEVEL_DEBUG;
        }
        else if (kv.second == "info")
        {
            iter->second._level = LOG_LEVEL_INFO;
        }
        else if (kv.second == "warn" || kv.second == "warning")
        {
            iter->second._level = LOG_LEVEL_WARN;
        }
        else if (kv.second == "error")
        {
            iter->second._level = LOG_LEVEL_ERROR;
        }
        else if (kv.second == "alarm")
        {
            iter->second._level = LOG_LEVEL_ALARM;
        }
        else if (kv.second == "fatal")
        {
            iter->second._level = LOG_LEVEL_FATAL;
        }
    }
    //! display
    else if (kv.first == "display")
    {
        if (kv.second == "false" || kv.second == "0")
        {
            iter->second._display = false;
        }
        else
        {
            iter->second._display = true;
        }
    }
    //! output to file
    else if (kv.first == "outfile")
    {
        if (kv.second == "false" || kv.second == "0")
        {
            iter->second._outfile = false;
        }
        else
        {
            iter->second._outfile = true;
        }
    }
    //! monthdir
    else if (kv.first == "monthdir")
    {
        if (kv.second == "false" || kv.second == "0")
        {
            iter->second._monthdir = false;
        }
        else
        {
            iter->second._monthdir = true;
        }
    }
    //! limit file size
    else if (kv.first == "limitsize")
    {
        iter->second._limitsize = atoi(kv.second.c_str());
    }
    //! display log in file line
    else if (kv.first == "fileline")
    {
        if (kv.second == "false" || kv.second == "0")
        {
            iter->second._fileLine = false;
        }
        else
        {
            iter->second._fileLine = true;
        }
    }
    //! enable/disable one logger
    else if (kv.first == "enable")
    {
        if (kv.second == "false" || kv.second == "0")
        {
            iter->second._enable = false;
        }
        else
        {
            iter->second._enable = true;
        }
    }
    return true;
}

static bool parseConfigFromString(std::string content, std::map<std::string, LoggerInfo> & outInfo)
{
    
    std::string key;
    int curLine = 1;
    std::string line;
    std::string::size_type curPos = 0;
    if (content.empty())
    {
        return true;
    }
    do
    {
        std::string::size_type pos = std::string::npos;
        for (std::string::size_type i = curPos; i < content.length(); ++i)
        {
            //support linux/unix/windows LRCF
            if (content[i] == '\r' || content[i] == '\n')
            {
                pos = i;
                break;
            }
        }
        line = content.substr(curPos, pos - curPos);
        parseConfigLine(line, curLine, key, outInfo);
        curLine++;
        
        if (pos == std::string::npos)
        {
            break;
        }
        else
        {
            curPos = pos+1;
        }
    } while (1);
    return true;
}



bool isDirectory(std::string path)
{
#ifdef WIN32
    return PathIsDirectoryA(path.c_str()) ? true : false;
#else
    DIR * pdir = opendir(path.c_str());
    if (pdir == NULL)
    {
        return false;
    }
    else
    {
        closedir(pdir);
        pdir = NULL;
        return true;
    }
#endif
}



bool createRecursionDir(std::string path)
{
    if (path.length() == 0) return true;
    std::string sub;
    fixPath(path);
    
    std::string::size_type pos = path.find('/');
    while (pos != std::string::npos)
    {
        std::string cur = path.substr(0, pos-0);
        if (cur.length() > 0 && !isDirectory(cur))
        {
            bool ret = false;
#ifdef WIN32
            ret = CreateDirectoryA(cur.c_str(), NULL) ? true : false;
#else
            ret = (mkdir(cur.c_str(), S_IRWXU|S_IRWXG|S_IRWXO) == 0);
#endif
            if (!ret)
            {
                return false;
            }
        }
        pos = path.find('/', pos+1);
    }
    
    return true;
}

std::string getProcessID()
{
    std::string pid = "0";
    char buf[260] = {0};
#ifdef WIN32
    DWORD winPID = GetCurrentProcessId();
    sprintf(buf, "%06d", winPID);
    pid = buf;
#else
    sprintf(buf, "%06d", getpid());
    pid = buf;
#endif
    return pid;
}


std::string getProcessName()
{
    std::string name = "MainLog";
    char buf[260] = {0};
#ifdef WIN32
    if (GetModuleFileNameA(NULL, buf, 259) > 0)
    {
        name = buf;
    }
    std::string::size_type pos = name.rfind("\\");
    if (pos != std::string::npos)
    {
        name = name.substr(pos+1, std::string::npos);
    }
    pos = name.rfind(".");
    if (pos != std::string::npos)
    {
        name = name.substr(0, pos-0);
    }
    
#elif defined(__APPLE__)
    
    proc_name(getpid(), buf, 260);
    name = buf;
    return name;;
#else
    sprintf(buf, "/proc/%d/cmdline", (int)getpid());
    Log4zFileHandler i;
    i.open(buf, "r");
    if (!i.isOpen())
    {
        return name;
    }
    name = i.readLine();
    i.close();
    
    std::string::size_type pos = name.rfind("/");
    if (pos != std::string::npos)
    {
        name = name.substr(pos+1, std::string::npos);
    }
#endif
    
    
    return name;
}






//////////////////////////////////////////////////////////////////////////
// LockHelper
//////////////////////////////////////////////////////////////////////////
LockHelper::LockHelper()
{
#ifdef WIN32
    InitializeCriticalSection(&_crit);
#else
    //_crit = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_crit, &attr);
    pthread_mutexattr_destroy(&attr);
#endif
}
LockHelper::~LockHelper()
{
#ifdef WIN32
    DeleteCriticalSection(&_crit);
#else
    pthread_mutex_destroy(&_crit);
#endif
}

void LockHelper::lock()
{
#ifdef WIN32
    EnterCriticalSection(&_crit);
#else
    pthread_mutex_lock(&_crit);
#endif
}
void LockHelper::unLock()
{
#ifdef WIN32
    LeaveCriticalSection(&_crit);
#else
    pthread_mutex_unlock(&_crit);
#endif
}
//////////////////////////////////////////////////////////////////////////
// SemHelper
//////////////////////////////////////////////////////////////////////////
SemHelper::SemHelper()
{
#ifdef WIN32
    _hSem = NULL;
#elif defined(__APPLE__)
    _semid = NULL;
#else
    _isCreate = false;
#endif
    
}
SemHelper::~SemHelper()
{
#ifdef WIN32
    if (_hSem != NULL)
    {
        CloseHandle(_hSem);
        _hSem = NULL;
    }
#elif defined(__APPLE__)
    if (_semid)
    {
        dispatch_release(_semid);
        _semid = NULL;
    }
#else
    if (_isCreate)
    {
        _isCreate = false;
        sem_destroy(&_semid);
    }
#endif
    
}

bool SemHelper::create(int initcount)
{
    if (initcount < 0)
    {
        initcount = 0;
    }
#ifdef WIN32
    if (initcount > 64)
    {
        return false;
    }
    _hSem = CreateSemaphore(NULL, initcount, 64, NULL);
    if (_hSem == NULL)
    {
        return false;
    }
#elif defined(__APPLE__)
    _semid = dispatch_semaphore_create(initcount);
    if (!_semid)
    {
        return false;
    }
#else
    if (sem_init(&_semid, 0, initcount) != 0)
    {
        return false;
    }
    _isCreate = true;
#endif
    
    return true;
}
bool SemHelper::wait(int timeout)
{
#ifdef WIN32
    if (timeout <= 0)
    {
        timeout = INFINITE;
    }
    if (WaitForSingleObject(_hSem, timeout) != WAIT_OBJECT_0)
    {
        return false;
    }
#elif defined(__APPLE__)
    if (dispatch_semaphore_wait(_semid, dispatch_time(DISPATCH_TIME_NOW, timeout*1000)) != 0)
    {
        return false;
    }
#else
    if (timeout <= 0)
    {
        return (sem_wait(&_semid) == 0);
    }
    else
    {
        struct timeval tm;
        gettimeofday(&tm, NULL);
        long long endtime = tm.tv_sec *1000 + tm.tv_usec/1000 + timeout;
        do
        {
            sleepMillisecond(50);
            int ret = sem_trywait(&_semid);
            if (ret == 0)
            {
                return true;
            }
            struct timeval tv_cur;
            gettimeofday(&tv_cur, NULL);
            if (tv_cur.tv_sec*1000 + tv_cur.tv_usec/1000 > endtime)
            {
                return false;
            }
            
            if (ret == -1 && errno == EAGAIN)
            {
                continue;
            }
            else
            {
                return false;
            }
        } while (true);
        return false;
    }
#endif
    return true;
}

bool SemHelper::post()
{
#ifdef WIN32
    return ReleaseSemaphore(_hSem, 1, NULL) ? true : false;
#elif defined(__APPLE__)
    return dispatch_semaphore_signal(_semid) == 0;
#else
    return (sem_post(&_semid) == 0);
#endif
    
}

//////////////////////////////////////////////////////////////////////////
//! ThreadHelper
//////////////////////////////////////////////////////////////////////////
bool ThreadHelper::start()
{
#ifdef WIN32
    unsigned long long ret = _beginthreadex(NULL, 0, threadProc, (void *) this, 0, NULL);
    
    if (ret == -1 || ret == 0)
    {
        std::cout << "log4z: create log4z thread error! \r\n" <<std::endl;
        return false;
    }
    _hThreadID = ret;
#else
    int ret = pthread_create(&_phtreadID, NULL, threadProc, (void*)this);
    if (ret != 0)
    {
        std::cout <<"log4z: create log4z thread error! \r\n" << std::endl;
        return false;
    }
#endif
    return true;
}

bool ThreadHelper::wait()
{
#ifdef WIN32
    if (WaitForSingleObject((HANDLE)_hThreadID, INFINITE) != WAIT_OBJECT_0)
    {
        return false;
    }
#else
    if (pthread_join(_phtreadID, NULL) != 0)
    {
        return false;
    }
#endif
    return true;
}

//////////////////////////////////////////////////////////////////////////
//! LogerManager
//////////////////////////////////////////////////////////////////////////
LogerManager::LogerManager()
{
    _runing = false;
    _lastId = LOG4Z_MAIN_LOGGER_ID;
    _hotUpdateInterval = 0;
    
    _ullStatusTotalPushLog = 0;
    _ullStatusTotalPopLog = 0;
    _ullStatusTotalWriteFileCount = 0;
    _ullStatusTotalWriteFileBytes = 0;
    
    _pid = getProcessID();
    _proName = getProcessName();
    _loggers[LOG4Z_MAIN_LOGGER_ID]._enable = true;
    _ids[LOG4Z_MAIN_LOGGER_KEY] = LOG4Z_MAIN_LOGGER_ID;
    _loggers[LOG4Z_MAIN_LOGGER_ID]._key = LOG4Z_MAIN_LOGGER_KEY;
    _loggers[LOG4Z_MAIN_LOGGER_ID]._name = _proName;
    
}
LogerManager::~LogerManager()
{
    stop();
}




void LogerManager::showColorText(const char *text, int level)
{
    if (level <= LOG_LEVEL_DEBUG || level > LOG_LEVEL_FATAL)
    {
        printf("%s", text);
        return;
    }
#ifndef WIN32
    printf("%s%s\e[0m", LOG_COLOR[level], text);
#else
    
    AutoLock l(_scLock);
    HANDLE hStd = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStd == INVALID_HANDLE_VALUE) return;
    CONSOLE_SCREEN_BUFFER_INFO oldInfo;
    if (!GetConsoleScreenBufferInfo(hStd, &oldInfo))
    {
        return;
    }
    else
    {
        SetConsoleTextAttribute(hStd, LOG_COLOR[level]);
        printf("%s", text);
        SetConsoleTextAttribute(hStd, oldInfo.wAttributes);
    }
#endif
    return;
}

bool LogerManager::configFromStringImpl(std::string content, bool isUpdate)
{
    std::map<std::string, LoggerInfo> loggerMap;
    if (!parseConfigFromString(content, loggerMap))
    {
        std::cout << " !!! !!! !!! !!!" << std::endl;
        std::cout << " !!! !!! log4z load config file error" << std::endl;
        std::cout << " !!! !!! !!! !!!" << std::endl;
        return false;
    }
    for (std::map<std::string, LoggerInfo>::iterator iter = loggerMap.begin(); iter != loggerMap.end(); ++iter)
    {
        LoggerId id = LOG4Z_INVALID_LOGGER_ID;
        id = findLogger(iter->second._key.c_str());
        if (id == LOG4Z_INVALID_LOGGER_ID)
        {
            if (isUpdate)
            {
                continue;
            }
            else
            {
                id = createLogger(iter->second._key.c_str());
                if (id == LOG4Z_INVALID_LOGGER_ID)
                {
                    continue;
                }
            }
        }
        enableLogger(id, iter->second._enable);
        setLoggerName(id, iter->second._name.c_str());
        setLoggerPath(id, iter->second._path.c_str());
        setLoggerLevel(id, iter->second._level);
        setLoggerFileLine(id, iter->second._fileLine);
        setLoggerDisplay(id, iter->second._display);
        setLoggerOutFile(id, iter->second._outfile);
        setLoggerLimitsize(id, iter->second._limitsize);
        setLoggerMonthdir(id, iter->second._monthdir);
    }
    return true;
}

//! read configure and create with overwriting
bool LogerManager::config(const char* configPath)
{
    if (!_configFile.empty())
    {
        std::cout << " !!! !!! !!! !!!" << std::endl;
        std::cout << " !!! !!! log4z configure error: too many calls to Config. the old config file=" << _configFile << ", the new config file=" << configPath << " !!! !!! " << std::endl;
        std::cout << " !!! !!! !!! !!!" << std::endl;
        return false;
    }
    _configFile = configPath;
    
    Log4zFileHandler f;
    f.open(_configFile.c_str(), "r");
    if (!f.isOpen())
    {
        std::cout << " !!! !!! !!! !!!" << std::endl;
        std::cout << " !!! !!! log4z load config file error. filename=" << configPath << " !!! !!! " << std::endl;
        std::cout << " !!! !!! !!! !!!" << std::endl;
        return false;
    }
    return configFromStringImpl(f.readContent().c_str(), false);
}

//! read configure and create with overwriting
bool LogerManager::configFromString(const char* configContent)
{
    return configFromStringImpl(configContent, false);
}

//! create with overwriting
LoggerId LogerManager::createLogger(const char* key)
{
    if (key == NULL)
    {
        return LOG4Z_INVALID_LOGGER_ID;
    }
    
    std::string copyKey = key;
    trimLogConfig(copyKey);
    
    LoggerId newID = LOG4Z_INVALID_LOGGER_ID;
    {
        std::map<std::string, LoggerId>::iterator iter = _ids.find(copyKey);
        if (iter != _ids.end())
        {
            newID = iter->second;
        }
    }
    if (newID == LOG4Z_INVALID_LOGGER_ID)
    {
        if (_lastId +1 >= LOG4Z_LOGGER_MAX)
        {
            showColorText("log4z: CreateLogger can not create|writeover, because loggerid need < LOGGER_MAX! \r\n", LOG_LEVEL_FATAL);
            return LOG4Z_INVALID_LOGGER_ID;
        }
        newID = ++ _lastId;
        _ids[copyKey] = newID;
        _loggers[newID]._enable = true;
        _loggers[newID]._key = copyKey;
        _loggers[newID]._name = copyKey;
    }
    
    return newID;
}


bool LogerManager::start()
{
    if (_runing)
    {
        return false;
    }
    _semaphore.create(0);
    bool ret = ThreadHelper::start();
    return ret && _semaphore.wait(3000);
}
bool LogerManager::stop()
{
    if (_runing == true)
    {
        _runing = false;
        wait();
        return true;
    }
    return false;
}
bool LogerManager::prePushLog(LoggerId id, int level)
{
    if (id < 0 || id > _lastId || !_runing || !_loggers[id]._enable)
    {
        return false;
    }
    if (level < _loggers[id]._level)
    {
        return false;
    }
    return true;
}
bool LogerManager::pushLog(LoggerId id, int level, const char * log, const char * file, int line)
{
    // discard log
    if (id < 0 || id > _lastId || !_runing || !_loggers[id]._enable)
    {
        return false;
    }
    
    //filter log
    if (level < _loggers[id]._level)
    {
        return false;
    }
    
    //create log data
    LogData * pLog = new LogData;
    pLog->_id =id;
    pLog->_level = level;
    
    //append precise time to log
    {
#ifdef WIN32
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        unsigned long long now = ft.dwHighDateTime;
        now <<= 32;
        now |= ft.dwLowDateTime;
        now /=10;
        now -=11644473600000000ULL;
        now /=1000;
        pLog->_time = now/1000;
        pLog->_precise = (unsigned int)(now%1000);
#else
        struct timeval tm;
        gettimeofday(&tm, NULL);
        pLog->_time = tm.tv_sec;
        pLog->_precise = tm.tv_usec/1000;
#endif
    }
    
    //format log
    {
        tm tt = timeToTm(pLog->_time);
        if (file == NULL || !_loggers[pLog->_id]._fileLine)
        {
#ifdef WIN32
            int ret = _snprintf_s(pLog->_content, LOG4Z_LOG_BUF_SIZE, _TRUNCATE, "%d-%02d-%02d %02d:%02d:%02d.%03d %s %s \r\n",
                                  tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, pLog->_precise,
                                  LOG_STRING[pLog->_level], log);
            if (ret == -1)
            {
                ret = LOG4Z_LOG_BUF_SIZE - 1;
            }
            pLog->_contentLen = ret;
#else
            int ret = snprintf(pLog->_content, LOG4Z_LOG_BUF_SIZE, "%d-%02d-%02d %02d:%02d:%02d.%03d %s %s \r\n",
                               tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, pLog->_precise,
                               LOG_STRING[pLog->_level], log);
            if (ret == -1)
            {
                ret = 0;
            }
            if (ret >= LOG4Z_LOG_BUF_SIZE)
            {
                ret = LOG4Z_LOG_BUF_SIZE-1;
            }
            
            pLog->_contentLen = ret;
#endif
        }
        else
        {
            const char * pNameBegin = file+strlen(file);
            do
            {
                if (*pNameBegin == '\\' || *pNameBegin == '/'){ pNameBegin++; break;}
                if (pNameBegin == file){break;}
                pNameBegin--;
            } while (true);
            
            
#ifdef WIN32
            int ret = _snprintf_s(pLog->_content, LOG4Z_LOG_BUF_SIZE, _TRUNCATE, "%d-%02d-%02d %02d:%02d:%02d.%03d %s %s (%s):%d \r\n",
                                  tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, pLog->_precise,
                                  LOG_STRING[pLog->_level], log, pNameBegin, line);
            if (ret == -1)
            {
                ret = LOG4Z_LOG_BUF_SIZE - 1;
            }
            pLog->_contentLen = ret;
#else
            int ret = snprintf(pLog->_content, LOG4Z_LOG_BUF_SIZE, "%d-%02d-%02d %02d:%02d:%02d.%03d %s %s (%s):%d \r\n",
                               tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, pLog->_precise,
                               LOG_STRING[pLog->_level], log, pNameBegin, line);
            if (ret == -1)
            {
                ret = 0;
            }
            if (ret >= LOG4Z_LOG_BUF_SIZE)
            {
                ret = LOG4Z_LOG_BUF_SIZE-1;
            }
            
            pLog->_contentLen = ret;
#endif
        }
        
        if (pLog->_contentLen >= 2)
        {
            pLog->_content[pLog->_contentLen - 2] = '\r';
            pLog->_content[pLog->_contentLen - 1] = '\n';
        }
        
    }
    
    if (_loggers[pLog->_id]._display && LOG4Z_ALL_SYNCHRONOUS_OUTPUT)
    {
        showColorText(pLog->_content, pLog->_level);
    }
    
    if (LOG4Z_ALL_DEBUGOUTPUT_DISPLAY && LOG4Z_ALL_SYNCHRONOUS_OUTPUT)
    {
#ifdef WIN32
        OutputDebugStringA(pLog->_content);
#endif
    }
    
    if (_loggers[pLog->_id]._outfile && LOG4Z_ALL_SYNCHRONOUS_OUTPUT)
    {
        AutoLock l(_logLock);
        if (openLogger(pLog))
        {
            _loggers[pLog->_id]._handle.write(pLog->_content, pLog->_contentLen);
            closeLogger(pLog->_id);
            _ullStatusTotalWriteFileCount++;
            _ullStatusTotalWriteFileBytes += pLog->_contentLen;
        }
    }
    
    if (LOG4Z_ALL_SYNCHRONOUS_OUTPUT)
    {
        delete pLog;
        return true;
    }
    
    AutoLock l(_logLock);
    _logs.push_back(pLog);
    _ullStatusTotalPushLog ++;
    return true;
}

//! 查找ID
LoggerId LogerManager::findLogger(const char * key)
{
    std::map<std::string, LoggerId>::iterator iter;
    iter = _ids.find(key);
    if (iter != _ids.end())
    {
        return iter->second;
    }
    return LOG4Z_INVALID_LOGGER_ID;
}
bool LogerManager::enableLogger(LoggerId id, bool enable)
{
    if (id <0 || id > _lastId) return false;
    _loggers[id]._enable = enable;
    if (enable)
    {
        _loggers[id]._hotChange = true;
    }
    return true;
}
bool LogerManager::setLoggerLevel(LoggerId id, int level)
{
    if (id <0 || id > _lastId || level < LOG_LEVEL_TRACE || level >LOG_LEVEL_FATAL) return false;
    _loggers[id]._level = level;
    return true;
}
bool LogerManager::setLoggerDisplay(LoggerId id, bool enable)
{
    if (id <0 || id > _lastId) return false;
    _loggers[id]._display = enable;
    return true;
}
bool LogerManager::setLoggerOutFile(LoggerId id, bool enable)
{
    if (id <0 || id > _lastId) return false;
    _loggers[id]._outfile = enable;
    return true;
}
bool LogerManager::setLoggerMonthdir(LoggerId id, bool enable)
{
    if (id <0 || id > _lastId) return false;
    _loggers[id]._monthdir = enable;
    return true;
}
bool LogerManager::setLoggerLimitsize(LoggerId id, unsigned int limitsize)
{
    if (id <0 || id > _lastId) return false;
    if (limitsize == 0 ) {limitsize = (unsigned int)-1;}
    _loggers[id]._limitsize = limitsize;
    return true;
}
bool LogerManager::setLoggerFileLine(LoggerId id, bool enable)
{
    if (id <0 || id > _lastId) return false;
    _loggers[id]._fileLine = enable;
    return true;
}

bool LogerManager::setLoggerName(LoggerId id, const char * name)
{
    if (id <0 || id > _lastId) return false;
    //the name by main logger is the process name and it's can't change.
    if (id == LOG4Z_MAIN_LOGGER_ID) return false;
    
    if (name == NULL || strlen(name) == 0)
    {
        return false;
    }
    AutoLock l(_hotLock);
    if (_loggers[id]._name != name)
    {
        _loggers[id]._name = name;
        _loggers[id]._hotChange = true;
    }
    return true;
}

bool LogerManager::setLoggerPath(LoggerId id, const char * path)
{
    if (id <0 || id > _lastId) return false;
    std::string copyPath;
    if (path == NULL || strlen(path) == 0)
    {
        copyPath = LOG4Z_DEFAULT_PATH;
    }
    else
    {
        copyPath = path;
    }
    
    {
        char ch = copyPath.at(copyPath.length() - 1);
        if (ch != '\\' && ch != '/')
        {
            copyPath.append("/");
        }
    }
    
    AutoLock l(_hotLock);
    if (copyPath != _loggers[id]._path)
    {
        _loggers[id]._path = copyPath;
        _loggers[id]._hotChange = true;
    }
    return true;
}
bool LogerManager::setAutoUpdate(int interval)
{
    _hotUpdateInterval = interval;
    return true;
}
bool LogerManager::updateConfig()
{
    if (_configFile.empty())
    {
        //LOGW("log4z update config file error. filename is empty.");
        return false;
    }
    Log4zFileHandler f;
    f.open(_configFile.c_str(), "r");
    if (!f.isOpen())
    {
        std::cout << " !!! !!! !!! !!!" << std::endl;
        std::cout << " !!! !!! log4z load config file error. filename=" << _configFile << " !!! !!! " << std::endl;
        std::cout << " !!! !!! !!! !!!" << std::endl;
        return false;
    }
    return configFromStringImpl(f.readContent().c_str(), true);
}

bool LogerManager::isLoggerEnable(LoggerId id)
{
    if (id <0 || id > _lastId) return false;
    return _loggers[id]._enable;
}

unsigned int LogerManager::getStatusActiveLoggers()
{
    unsigned int actives = 0;
    for (int i=0; i<= _lastId; i++)
    {
        if (_loggers[i]._enable)
        {
            actives ++;
        }
    }
    return actives;
}


bool LogerManager::openLogger(LogData * pLog)
{
    int id = pLog->_id;
    if (id < 0 || id >_lastId)
    {
        showColorText("log4z: openLogger can not open, invalide logger id! \r\n", LOG_LEVEL_FATAL);
        return false;
    }
    
    LoggerInfo * pLogger = &_loggers[id];
    if (!pLogger->_enable || !pLogger->_outfile || pLog->_level < pLogger->_level)
    {
        return false;
    }
    
    bool sameday = isSameDay(pLog->_time, pLogger->_curFileCreateTime);
    bool needChageFile = pLogger->_curWriteLen > pLogger->_limitsize * 1024 * 1024;
    if (!sameday || needChageFile || pLogger->_hotChange)
    {
        if (!sameday || pLogger->_hotChange)
        {
            pLogger->_curFileIndex = 0;
        }
        else
        {
            pLogger->_curFileIndex++;
        }
        if (pLogger->_handle.isOpen())
        {
            pLogger->_handle.close();
        }
    }
    if (!pLogger->_handle.isOpen())
    {
        pLogger->_curFileCreateTime = pLog->_time;
        pLogger->_curWriteLen = 0;
        
        tm t = timeToTm(pLogger->_curFileCreateTime);
        std::string name;
        std::string path;
        _hotLock.lock();
        name = pLogger->_name;
        path = pLogger->_path;
        pLogger->_hotChange = false;
        _hotLock.unLock();
        
        char buf[100] = { 0 };
        if (pLogger->_monthdir)
        {
            sprintf(buf, "%04d_%02d/", t.tm_year + 1900, t.tm_mon + 1);
            path += buf;
        }
        
        if (!isDirectory(path))
        {
            createRecursionDir(path);
        }
        
        sprintf(buf, "%s_%04d%02d%02d%02d%02d_%s_%03d.log",
                name.c_str(), t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                t.tm_hour, t.tm_min, _pid.c_str(), pLogger->_curFileIndex);
        path += buf;
        pLogger->_handle.open(path.c_str(), "ab");
        if (!pLogger->_handle.isOpen())
        {
            pLogger->_outfile = false;
            return false;
        }
        return true;
    }
    return true;
}
bool LogerManager::closeLogger(LoggerId id)
{
    if (id < 0 || id >_lastId)
    {
        showColorText("log4z: closeLogger can not close, invalide logger id! \r\n", LOG_LEVEL_FATAL);
        return false;
    }
    LoggerInfo * pLogger = &_loggers[id];
    if (pLogger->_handle.isOpen())
    {
        pLogger->_handle.close();
        return true;
    }
    return false;
}
bool LogerManager::popLog(LogData *& log)
{
    AutoLock l(_logLock);
    if (_logs.empty())
    {
        return false;
    }
    log = _logs.front();
    _logs.pop_front();
    return true;
}

void LogerManager::run()
{
    _runing = true;
    pushLog(0, LOG_LEVEL_ALARM, "-----------------  log4z thread started!   ----------------------------", NULL, 0);
    for (int i = 0; i <= _lastId; i++)
    {
        if (_loggers[i]._enable)
        {
            std::stringstream ss;
            ss <<"logger id=" <<i 
            <<" key=" <<_loggers[i]._key
            <<" name=" <<_loggers[i]._name
            <<" path=" <<_loggers[i]._path
            <<" level=" << _loggers[i]._level
            <<" display=" << _loggers[i]._display;
            pushLog(0, LOG_LEVEL_ALARM, ss.str().c_str(), NULL, 0);
        }
    }
    
    _semaphore.post();
    
    
    LogData * pLog = NULL;
    int needFlush[LOG4Z_LOGGER_MAX] = {0};
    time_t lastCheckUpdate = time(NULL);
    while (true)
    {
        while(popLog(pLog))
        {
            //
            _ullStatusTotalPopLog ++;
            //discard
            LoggerInfo & curLogger = _loggers[pLog->_id];
            if (!curLogger._enable || pLog->_level <curLogger._level  )
            {
                delete pLog;
                pLog = NULL;
                continue;
            }
            
            
            if (curLogger._display && !LOG4Z_ALL_SYNCHRONOUS_OUTPUT)
            {
                showColorText(pLog->_content, pLog->_level);
            }
            if (LOG4Z_ALL_DEBUGOUTPUT_DISPLAY && !LOG4Z_ALL_SYNCHRONOUS_OUTPUT)
            {
#ifdef WIN32
                OutputDebugStringA(pLog->_content);
#endif
            }
            
            
            if (curLogger._outfile && !LOG4Z_ALL_SYNCHRONOUS_OUTPUT)
            {
                if (!openLogger(pLog))
                {
                    delete pLog;
                    pLog = NULL;
                    continue;
                }
                
                curLogger._handle.write(pLog->_content, pLog->_contentLen);
                curLogger._curWriteLen += (unsigned int)pLog->_contentLen;
                needFlush[pLog->_id] ++;
                _ullStatusTotalWriteFileCount++;
                _ullStatusTotalWriteFileBytes += pLog->_contentLen;
            }
            else if (!LOG4Z_ALL_SYNCHRONOUS_OUTPUT)
            {
                _ullStatusTotalWriteFileCount++;
                _ullStatusTotalWriteFileBytes += pLog->_contentLen;
            }
            
            delete pLog;
            pLog = NULL;
        }
        
        for (int i=0; i<=_lastId; i++)
        {
            if (_loggers[i]._enable && needFlush[i] > 0)
            {
                _loggers[i]._handle.flush();
                needFlush[i] = 0;
            }
            if(!_loggers[i]._enable && _loggers[i]._handle.isOpen())
            {
                _loggers[i]._handle.close();
            }
        }
        
        //! delay. 
        sleepMillisecond(100);
        
        //! quit
        if (!_runing && _logs.empty())
        {
            break;
        }
        
        if (_hotUpdateInterval != 0 && time(NULL) - lastCheckUpdate > _hotUpdateInterval)
        {
            updateConfig();
            lastCheckUpdate = time(NULL);
        }
        
        
        
    }
    
    for (int i=0; i <= _lastId; i++)
    {
        if (_loggers[i]._enable)
        {
            _loggers[i]._enable = false;
            closeLogger(i);
        }
    }
    
}

//////////////////////////////////////////////////////////////////////////
//ILog4zManager::getInstance
//////////////////////////////////////////////////////////////////////////
ILog4zManager * ILog4zManager::getInstance()
{
    static LogerManager m;
    return &m;
}



_LOG4Z_END

