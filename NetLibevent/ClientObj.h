#pragma once
#include "ConnObj.h"
#include "ExportDefine.h"
class ClientObj :public Util::NetEvent::CNetEvent
{
public:
	ClientObj();
	~ClientObj();
	Netfd Connect(const char* ip, int port);
	bool Send(const char* buf, int len);
	void DisConnect();
	void DoExit();
	virtual void OnConnect(evutil_socket_t fd, uint32_t uCode);
	virtual void OnWrite(evutil_socket_t fd);
	virtual void OnRead(evutil_socket_t fd, const char* buf, int len);

	//链接对象释放后通知删除
	virtual void OnFinishRelease(evutil_socket_t fd);
private:
	CConnObj* m_pObj;   //当前链接对象
};

