#pragma once
#include "EventBaseConn.h"
#include "ThreadBase.h"
#include <string>

namespace Util
{
	namespace NetEvent
	{
		class CNetEvent
		{
		public:
			virtual void OnConnect(evutil_socket_t fd, uint32_t uCode) = 0;
			virtual void OnWrite(evutil_socket_t fd) = 0;
			virtual void OnRead(evutil_socket_t fd, const char* buf, int len) = 0;
			//线程结束 释放资源
			virtual void OnFinishRelease(evutil_socket_t fd)=0;
		};
	}
}

//连接对象 保存建立链接的实体对象  一个conn对象映射一个fd
class CConnObj : public CThreadBase
{
public:
	CConnObj(Util::NetEvent::CNetEvent* pSink);
	~CConnObj();
	//一般服务端使用
	void Init(evutil_socket_t fd);
	//一般客户端使用 内部生成fd
	evutil_socket_t Init(const char* ip, int port);

	virtual void Run_Thread(void* param);
	virtual void OnFinish_Thread();
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);

	void SetFdCallBack(evutil_socket_t fd, bool bCbRead, bool bCbWrite);
	bool Send(const char* buf, int len);
	void Stop();

protected:
	void OnRead(bufferevent *bev);
	void OnWrite(bufferevent *bev);
	void OnConnect(bufferevent *bev, short events);
private:
	Util::NetEvent::CNetEvent* m_pSink;
	CEventBaseConn* m_pCon;  //链接对象
	evutil_socket_t m_fd;
	std::string m_ip;
	int m_port;
};

