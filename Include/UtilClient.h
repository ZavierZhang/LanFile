#pragma once
#include "ExportDefine.h"
//基于LibEvent 的客户端接口
namespace Util
{
	namespace LibEventClient
	{
		namespace EventClient
		{
			class CEventClient
			{
			public:
				virtual void OnConnect(int uCode,Netfd fd) = 0;
				virtual void OnFdWrite(Netfd fd) = 0;
				virtual void OnFdRead(Netfd fd, const char* szBuf, int len) = 0;
			};
		}
		 void Reg(Util::LibEventClient::EventClient::CEventClient* pSink);
		 Netfd Connect(const char* ip, int port);
		 bool Send(Netfd fd, const char* buf, int len);
		 void DisConnect(Netfd fd);
		//退出客户端连接模块  step1：关闭所有功能 即停止所有线程   step2：释放所有申请资源
		 void DoExit();
	}
}