#pragma once
#include <string>
#ifdef WIN32
#include "log4z.h"
#endif
namespace Util
{
	namespace Tool
	{
		//获取一个GUID
		std::string GetGuid();
	}
}
#ifdef WIN32
#define LOG_WIN_E(log) LOGE(log) 
#define LOG_WIN_I(log) LOGI(log)
#else
#define LOG_WIN_E(log) 
#define LOG_WIN_I(log) 
#endif


                                          
  

