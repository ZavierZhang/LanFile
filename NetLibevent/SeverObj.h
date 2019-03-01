#pragma once
#include "ExportDefine.h"
#include "ConnectQueue.h"
#include "ListenObj.h"
//服务器对象
class CSeverObj : public Util::ListenEvent::CListenEve,public Util::NetEvent::CNetEvent
{
public:
	CSeverObj();
	~CSeverObj();
	int Init(int port);
	void BeginSever(bool bBlock = false);
	void SetFdCallBack(Netfd fd, bool bCbRead, bool bCbWrite);
	bool Send(Netfd fd,const char* buf, int len);
	void Stop(Netfd fd);
	void EndSever();
	void UnInit();
	virtual void OnListen(bool bOK, int port);
	virtual void OnAccept(evutil_socket_t fd);
	virtual void OnConnect(evutil_socket_t fd, uint32_t uCode);
	virtual void OnWrite(evutil_socket_t fd);
	virtual void OnRead(evutil_socket_t fd,const char* buf,int len);

	virtual void OnFinishRelease(evutil_socket_t fd);
private:
	int m_port;
	CListenObj* m_plisten;  //监听对象
	CConnectQueue* m_pConnQueue;  //链接对象 管理链接队列
};

