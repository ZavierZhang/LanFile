#pragma once
#include "EventBase.h"
class CEventBaseConn : public CEventBase
{
public:
	CEventBaseConn();
	virtual ~CEventBaseConn();
	//fd 若为-1 则 内部构造fd,否则以传入fd为准   bConnFd是否是已经连接的fd 则SocketConn 接口将失效
	bufferevent* AttachSocket(evutil_socket_t connfd, int options);
	void AttachSocketCb(bufferevent_data_cb readpfn, bufferevent_data_cb writepfn, bufferevent_event_cb evepfn, void* arg);
	void SocketDisConn();
	void SetCallEve(short event);
	int GetConnType();
	//发送的数据由bufferevent接管
	bool Send(const char* buf, int len);
	int Read(char** buf, bufferevent* bev);
	virtual void Release();
protected:
	bufferevent* m_bev;
	int m_ConnType;
};
//客户端 的连接对象
class CBaseEventConnClient : public CEventBaseConn
{
public:
	CBaseEventConnClient();
	virtual~CBaseEventConnClient();
	//客户端连接
	int  SocketConn(const char* ip, int port);
};

