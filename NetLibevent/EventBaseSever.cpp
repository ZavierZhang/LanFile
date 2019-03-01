#include "stdafx.h"
#include "EventBaseSever.h"


CEventBaseSever::CEventBaseSever()
{
	SetType(EVENT_TYPE_SEVER);
	m_listen = NULL;
}
CEventBaseSever::~CEventBaseSever()
{
}
//����˼����ӿ�  ����listen ����  pfn listen �ص����� ptr userdata   port  ���������Ķ˿ں�
evconnlistener* CEventBaseSever::AttachListen(evconnlistener_cb pfn, void* ptr, unsigned flags, int port)
{
	if (NULL == m_base)
		return NULL;
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;      //Ŀǰֻ֧��IPV4
	sin.sin_port = htons(port);
	m_listen = evconnlistener_new_bind(m_base, pfn, (void *)ptr, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&sin, sizeof(sin));
	//��ȡ�󶨵Ķ˿ں�
	evutil_socket_t fd= evconnlistener_get_fd(m_listen);
	struct sockaddr_in connAddr;
	int len = sizeof(connAddr);
#ifdef WIN32
	if (0 != getsockname(fd, (SOCKADDR*)&connAddr, &len))
		return 0;
#else
	socklen_t len2 = sizeof(connAddr);
	if (0 != getsockname(fd, (struct sockaddr*)&connAddr, &len2))
		return 0;
#endif // WIN32

	
    m_Port= ntohs(connAddr.sin_port); // ��ȡ�˿ں�
	if (NULL == m_listen)
		return NULL;
	return m_listen;
}
int CEventBaseSever::GetListenPort()
{
	return m_Port;
}
void CEventBaseSever::Release()
{
	if (NULL != m_listen)
	{
		evconnlistener_free(m_listen);
	}
	if (NULL != m_base)
	{
		event_base_free(m_base);
	}
}
