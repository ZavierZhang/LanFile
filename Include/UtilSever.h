#pragma once
#include "ExportDefine.h"
//基于libevent的服务端接口
namespace Util
{
	namespace LibEventSever
	{
		class CBeginSeverRes
		{
		public:
			virtual void OnBeginRes(bool bOk, int port) = 0;
		};
		class CEventSever
		{
		public:
			//启动服务结果
			virtual void OnAccept(Netfd fd) = 0;
			//以下事件 回调每一个连接的fd 对应一个线程 上层应注意这一点 若有共享数据 则需加锁保护
			virtual void OnFdWrite(Netfd fd) = 0;
			virtual void OnFdRead(Netfd fd,const char* szBuf,int len) = 0;
			virtual void OnFdConnet(Netfd fd, int code) = 0;
		};
		 int Init(int port);
		 void Reg(Util::LibEventSever::CEventSever* pSink);
		 void RegBeginSever(Util::LibEventSever::CBeginSeverRes* pSink);
		 void BeginSever(bool bBlock=false);
		 void SetFdCallBack(Netfd fd,bool bCbRead, bool bCbWrite);
		 bool Send(Netfd fd, const char* buf, int len);
		 void Stop(Netfd fd);
		//停止服务器，
		 void EndSever();
		//释放资源
		 void UnInit();
	}
}

