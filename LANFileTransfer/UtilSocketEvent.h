#pragma once
#include <string>
namespace Util
{
	namespace SocketEvent
	{
		class CSocketEvent
		{
		public: 
			//接受socket连接请求  返回socketkey
			virtual void OnSocketAccept(UINT32 uKey) = 0;
			//客户端链接成功回调
			virtual void OnSocketConnect(UINT32 uKey,UINT32 uCode) = 0;
			//SOCKET 发送数据之间的回调 上层通过此回调填充数据  具体发送多次依赖底层网络
			virtual void OnSocketPreSend(UINT32 uKey,std::string& buf) = 0;
			//发送数据之后的回调
			virtual void OnSocketEndSend() = 0;
			//处理收数据之前回调
			virtual void OnSocketPreRecv(UINT32 uKey,UINT64& uLenBuff) = 0;
		    //处理收数据之后回调  接收数据的buf
			virtual void OnSocketEndRecv(UINT32 uKey,std::string& buf) = 0;
			//关闭数据
			virtual void OnSocketClose(UINT32 uKey) = 0;
		};
	}
}
