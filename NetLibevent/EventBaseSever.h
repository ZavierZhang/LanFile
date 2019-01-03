#pragma once
#include "EventBase.h"
#include "EventBaseSever.h"
class CEventBaseSever :public CEventBase
{
public:
	CEventBaseSever();
	~CEventBaseSever();
	//服务端监听接口  返回listen 对象  pfn listen 回调函数 ptr userdata   port  开发监听的端口号
	evconnlistener* AttachListen(evconnlistener_cb pfn, void* ptr, unsigned flags, int port);
	int GetListenPort();
	virtual void Release();
private:
	evconnlistener* m_listen;
	int m_Port;   //开放的端口号  如果外部不传入则是返回已绑定有端口号
};

