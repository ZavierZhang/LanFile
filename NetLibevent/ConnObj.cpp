#include "stdafx.h"
#include "ConnObj.h"
#define  ERROR_OK   0

CConnObj::CConnObj(Util::NetEvent::CNetEvent* pSink)
{
	m_pCon = NULL;
	m_pSink = pSink;
}


CConnObj::~CConnObj()
{
	if (NULL != m_pCon)
	{
		delete m_pCon;
	}
}

void CConnObj::Run_Thread(void* param)
{
	if (NULL == m_pCon)
		return;
	m_pCon->SetMultiThreadSupport(true);   //这边目前启动多线程支持会导致内存泄漏 原因目前未知 单这边单个文件传输 不需要多线程支持 
	m_pCon->CreateEventBase();
	m_pCon->AttachSocket(m_fd, BEV_OPT_CLOSE_ON_FREE);
	m_pCon->AttachSocketCb(conn_readcb, NULL, conn_eventcb, this);
	//需要链接的conn
	if (1 == m_pCon->GetConnType())
	{
		CBaseEventConnClient* p = (CBaseEventConnClient*)m_pCon;
		p->SocketConn(m_ip.c_str(), m_port);
	}
	m_pCon->SetCallEve(EV_READ | EV_WRITE);  //关注读写事件
	m_pCon->Loop();
}
void CConnObj::OnFinish_Thread()
{
	if(NULL!=m_pCon)
	   m_pCon->Release();
	if (NULL != m_pSink)
		m_pSink->OnFinishRelease(m_fd);
	delete this;
	
}
void CConnObj::conn_writecb(struct bufferevent *bev, void *user_data)
{
	CConnObj* pThis = (CConnObj*)user_data;
	if (NULL == pThis)
		return;
	pThis->OnWrite(bev);
}
void CConnObj::OnRead(bufferevent *bev)
{
	evutil_socket_t  fd = bufferevent_getfd(bev);
	char* szBuf = NULL;
	int len=m_pCon->Read(&szBuf, bev);
	if (NULL != szBuf && 0 != len)
	{
		if (NULL != m_pSink)
			m_pSink->OnRead(fd, szBuf, len);
	}
	delete szBuf;
}
void CConnObj::OnWrite(bufferevent *bev)
{
	evutil_socket_t  fd = bufferevent_getfd(bev);
	if (NULL != m_pSink)
	{
		m_pSink->OnWrite(fd);
	}
}
void CConnObj::OnConnect(bufferevent *bev,short events)
{
	int uCode = -1;
	if (events & BEV_EVENT_EOF)
	{
		uCode = BEV_EVENT_EOF;
	}
	else if (events & BEV_EVENT_CONNECTED)
	{
		uCode = ERROR_OK;
	}
	else if (events & BEV_EVENT_ERROR)
	{
		uCode = BEV_EVENT_ERROR;
	}
	else if (events &BEV_EVENT_TIMEOUT)
	{
		uCode = BEV_EVENT_TIMEOUT;
	}
	else
	{
		uCode = 9999;
	}
	evutil_socket_t  fd = bufferevent_getfd(bev);
	if (NULL != m_pSink)
	{
		m_pSink->OnConnect(fd,uCode);
	}
}
void CConnObj::conn_readcb(struct bufferevent *bev, void *user_data)
{
	CConnObj* pThis = (CConnObj*)user_data;
	if (NULL == pThis)
		return;
	pThis->OnRead(bev);
}
void CConnObj::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	CConnObj* pThis = (CConnObj*)user_data;
	if (NULL == pThis)
		return;
	pThis->OnConnect(bev, events);
}
void CConnObj::SetFdCallBack(evutil_socket_t fd, bool bCbRead, bool bCbWrite)
{
	if (NULL == m_pCon)
		return;
	bufferevent_data_cb readfn = NULL;
	bufferevent_data_cb writefn = NULL;
	if (bCbRead)
		readfn = conn_readcb;
	if (bCbWrite)
		writefn = conn_writecb;
	m_pCon->AttachSocketCb(readfn, writefn, conn_eventcb, this);
}
bool CConnObj::Send(const char* buf, int len)
{
	if(NULL!=m_pCon)
	   return m_pCon->Send(buf, len);
	return false;
}
void CConnObj::Stop()
{
	if(NULL!=m_pCon)
		m_pCon->QuitLoop();
}
void CConnObj::Init(evutil_socket_t fd)
{
#define IPMSG_DEFAULT_TCPBUFMAX		(256 * 1024)		// 256KB
	if (NULL == m_pCon)
	{
		m_pCon = new CEventBaseConn;
	}
	m_fd = fd;

	int flg = true;
	::setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flg, sizeof(flg));   //设置NO tcp delay
	for (int buf_size = IPMSG_DEFAULT_TCPBUFMAX; buf_size > 0; buf_size /= 2)        //设置sendbuf
	{
		if (::setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, (char*)&buf_size, sizeof(buf_size)) == 0)
			break;
	}

	CreateThread(this);
}
evutil_socket_t CConnObj::Init(const char* ip, int port)
{
	if (NULL == m_pCon)
	{
		m_pCon = new CBaseEventConnClient;
	}
	m_ip = ip;
	m_port = port;
	//创建FD 传入
	int hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == hSocket)
		return INVALID_SOCKET;    //这边就不开启线程
	m_fd = hSocket;
	//Socket 设置
	int flg = true;
    ::setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flg, sizeof(flg));   //设置NO tcp delay
	//socket 收buf 256K
	for (int buf_size = IPMSG_DEFAULT_TCPBUFMAX; buf_size > 0; buf_size /= 2)
	{
		if (::setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, (char*)&buf_size, sizeof(buf_size)) == 0)
		{
			break;
		}
	}
	CreateThread(this);
	return m_fd;
}