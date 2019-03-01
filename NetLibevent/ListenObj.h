#pragma once
#include"EventBaseSever.h"
#include"ThreadBase.h"

namespace  Util
{
	namespace ListenEvent
	{
		class CListenEve
		{
		public:
			virtual void OnAccept(evutil_socket_t fd) = 0;
			virtual void OnListen(bool bOK, int port)=0;
		};
	}
}
//�����������
class CListenObj :public CThreadBase
{
public:
	CListenObj();
	~CListenObj();
	void Reg(Util::ListenEvent::CListenEve* pSink);	//�����ص�
	static void listener_cb(struct evconnlistener * listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data);
	int Start(int Port);
	void Stop();
	virtual void Run_Thread(void* param);
	virtual void OnFinish_Thread();
protected:
	void OnAccept(evutil_socket_t fd);
private:
	int m_port;
	CEventBaseSever m_ListenEvent;  //listen�������
	Util::ListenEvent::CListenEve* m_pSink;
};

