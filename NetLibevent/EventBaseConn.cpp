#include "stdafx.h"
#include "EventBaseConn.h"



CEventBaseConn::CEventBaseConn()
{
	m_bev = NULL;
	m_ConnType = 0;
}
CEventBaseConn::~CEventBaseConn()
{
	int  x = 0;
	int c = 1;
	int u = c + x;
}
bufferevent* CEventBaseConn::AttachSocket(evutil_socket_t connfd, int options)
{
	if (NULL == m_base)
		return NULL;
	m_bev = bufferevent_socket_new(m_base, connfd, options);
	return NULL;
}
void CEventBaseConn::AttachSocketCb(bufferevent_data_cb readpfn, bufferevent_data_cb writepfn, bufferevent_event_cb evepfn, void* arg)
{
	if (NULL == m_bev)
		return;
	bufferevent_setcb(m_bev, readpfn, writepfn, evepfn, arg);
}
void CEventBaseConn::SetCallEve(short event)
{
	if (NULL == m_bev)
		return;
	bufferevent_enable(m_bev, event);
}
int CEventBaseConn::GetConnType()
{
	return m_ConnType;
}
bool CEventBaseConn::Send(const char* buf, int len)
{
	if (NULL == m_bev)
		return false;
	int code = bufferevent_write(m_bev, buf, len);
	if (0 != code)
		return false;
	return true;
}
//¶ÁÈ¡buffer
int CEventBaseConn::Read(char** buf, bufferevent* bev)
{
	if (bev != m_bev)
	{
		return -1;// ÑÏÖØ´íÎó
	}

	struct evbuffer *input = bufferevent_get_input(bev);
	size_t len = evbuffer_get_length(input);
	if (len > 0)
	{
		char* szBuf = new char[len];
		memset(szBuf, 0, len);
		int size = bufferevent_read(bev, szBuf, len);
		*buf = szBuf;
		return len;
	}
	return 0;
}
void CEventBaseConn::SocketDisConn()
{

}
void CEventBaseConn::Release()
{
	if (NULL != m_bev)
	{
		bufferevent_free(m_bev);
	}
	if (NULL != m_base)
	{
		event_base_free(m_base);
	}
}


CBaseEventConnClient::CBaseEventConnClient()
{
	m_ConnType = 1;
}
CBaseEventConnClient::~CBaseEventConnClient()
{
	int  x = 0;
	int c = 1;
	int u = c + x;
}

int  CBaseEventConnClient::SocketConn(const char* ip, int port)
{
	struct sockaddr_in srv;
	memset(&srv, 0, sizeof(srv));

#ifdef WIN32
	srv.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	evutil_inet_pton(AF_INET, ip, &srv.sin_addr);
#endif // WIN32

	srv.sin_family = AF_INET;
	srv.sin_port = htons(port);
	int flag = bufferevent_socket_connect(m_bev, (struct sockaddr *)&srv, sizeof(srv));
	return flag;
}
