#include "stdafx.h"
#include "ConnTestObj.h"
#include "ConnTestMgr.h"
#include "Singleton.h"
CConnTestObj::CConnTestObj()
{
#ifdef WIN32
	WSAData wsdata;
	WSAStartup(MAKEWORD(2, 0), &wsdata);
#endif // WIN32

	
}


CConnTestObj::~CConnTestObj()
{
}
 void CConnTestObj::Run_Thread(void* param)
{
	struct sockaddr_in srv;
	memset(&srv, 0, sizeof(srv));
#ifdef WIN32
	srv.sin_addr.S_un.S_addr = inet_addr(m_ip.c_str());
#endif // WIN32
	evutil_inet_pton(AF_INET, m_ip.c_str(), &srv.sin_addr);
	
	srv.sin_family = AF_INET;
	srv.sin_port = htons(m_port);
	//evthread_use_windows_threads();
	event_base* base = event_base_new();
	bufferevent* bev = bufferevent_socket_new(base, m_fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, NULL, NULL, conn_eventcb, NULL);
	bufferevent_socket_connect(bev, (struct sockaddr *)&srv, sizeof(srv));
	struct timeval  tv = { 1,0 };
	bufferevent_set_timeouts(bev, &tv, NULL);
	event_base_dispatch(base);
	event_base_free(base);
}
 void CConnTestObj::OnFinish_Thread()
 {
	 delete this;
 }
 void CConnTestObj::conn_eventcb(struct bufferevent *bev, short events, void *user_data)
 {
	 evutil_socket_t  fd = bufferevent_getfd(bev);
	 bool bOK = false;
	 if (events & BEV_EVENT_CONNECTED)
	 {
		 bOK = true;
	 }
	 bufferevent_free(bev);
	 Singleton<ConnTestMgr>::Instance().OnConn(fd, bOK);  //通知结果
 }
 evutil_socket_t CConnTestObj::ConnTest(const char*ip, int port)
 {
	 m_ip = ip;
	 m_port = port;
	 int hSocket = socket(AF_INET, SOCK_STREAM, 0);
	 if (INVALID_SOCKET == hSocket)
		 return INVALID_SOCKET; 
	 m_fd = hSocket;
	 CreateThread(this);
	 return m_fd;
 }