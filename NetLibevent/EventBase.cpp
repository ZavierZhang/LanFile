#include "stdafx.h"
#include "EventBase.h"


CEventBase::CEventBase()
{
	m_base = NULL;
	m_type = EVENT_TYPE_NONE;
#ifdef WIN32
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
}


CEventBase::~CEventBase()
{
#ifdef WIN32
	WSACleanup();
#endif
}
void CEventBase::SetType(EVENT_TYPE type)
{
	m_type = type;
}
EVENT_TYPE CEventBase::GetType()
{
	return m_type;
}
//设置是否多线程支持
void CEventBase::SetMultiThreadSupport(bool bSupport)
{
	if (bSupport)
	{
#ifdef WIN32
		evthread_use_windows_threads();   //开启多线程
#else
		evthread_use_pthreads();
#endif // WIN32

	}
}
event_base* CEventBase::CreateEventBase()
{
	if (NULL != m_base)
		return m_base;
	m_base=event_base_new();
	//struct event_config* cfg = event_config_new();
	//event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP);    //使用iocp
	//m_base = event_base_new_with_config(cfg);
	//event_config_free(cfg);
	//const char* pMethod = event_base_get_method(m_base);
	return m_base;
}
int CEventBase::QuitLoop()
{
	if (NULL == m_base)
		return -1;
	return event_base_loopexit(m_base, NULL);      //立即退出事件循环
	//return event_base_loopbreak(m_base);
}
int CEventBase::Loop()
{
	if (NULL == m_base)
		return -1;
	 int x=event_base_dispatch(m_base);
	 return x;
}








