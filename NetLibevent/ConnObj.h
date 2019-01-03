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
			//�߳̽��� �ͷ���Դ
			virtual void OnFinishRelease(evutil_socket_t fd)=0;
		};
	}
}

//���Ӷ��� ���潨�����ӵ�ʵ�����  һ��conn����ӳ��һ��fd
class CConnObj : public CThreadBase
{
public:
	CConnObj(Util::NetEvent::CNetEvent* pSink);
	~CConnObj();
	//һ������ʹ��
	void Init(evutil_socket_t fd);
	//һ��ͻ���ʹ�� �ڲ�����fd
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
	CEventBaseConn* m_pCon;  //���Ӷ���
	evutil_socket_t m_fd;
	std::string m_ip;
	int m_port;
};

