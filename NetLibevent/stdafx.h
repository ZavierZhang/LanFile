// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifdef WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

#include <stdint.h>
#else
// mac 头文件
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <stddef.h>
#define  INVALID_SOCKET -1
#endif // WIN32





// TODO:  在此处引用程序需要的其他头文件
