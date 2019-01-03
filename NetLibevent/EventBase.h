#pragma once
//evet base 对libevent 相关的封装
//libevent 文件相关
#include <event2/util.h>
#include <event2/listener.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/thread.h>
#include <string>
enum EVENT_TYPE
{
	EVENT_TYPE_NONE=0,
	EVENT_TYPE_SEVER=1,
	EVENT_TYPE_CLIENT=2,
};
class CEventBase
{
public:
	CEventBase();
	virtual ~CEventBase();
	void SetType(EVENT_TYPE type);
	EVENT_TYPE GetType();
	//设置是否多线程支持
	void SetMultiThreadSupport(bool bSupport);
	event_base* CreateEventBase();
	int QuitLoop();
	int Loop();
	virtual void Release()=0;  //资源释放
protected:
	event_base* m_base;
	EVENT_TYPE m_type;
};




