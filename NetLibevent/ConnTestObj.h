#pragma once
#include "EventBase.h"
#include "ThreadBase.h"
class CConnTestObj : public CThreadBase
{
public:
	CConnTestObj();
	~CConnTestObj();
	virtual void Run_Thread(void* param);
	virtual void OnFinish_Thread();
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
	evutil_socket_t ConnTest(const char*ip, int port);
private:
	std::string m_ip;
	int m_port;
	evutil_socket_t m_fd;
};

