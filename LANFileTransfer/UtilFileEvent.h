#pragma once
#include"PkgDefine.h"
#include <string>
namespace Util
{
	namespace FILE_EVENT
	{
		class CFileEvent
		{
		public:
			//文件通讯命令
			virtual void OnFileCmd(FileSign cmd, uint32_t uKey, std::string& buf) = 0;
			//文件发送回调  服务端使用
			virtual void OnFileSend(uint32_t uKey)=0;
			//文件接收回调 客户端使用
			virtual void OnFileRecive(uint32_t uKey, std::string& buf) = 0;
			//底层连接异常了 
			virtual void OnFileError(uint32_t uKey, uint32_t uCode) = 0;
		};
	}
}
