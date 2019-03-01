#pragma once
#include "EventBase.h"
class CEventBaseConn : public CEventBase
{
public:
	CEventBaseConn();
	virtual ~CEventBaseConn();
	//fd ��Ϊ-1 �� �ڲ�����fd,�����Դ���fdΪ׼   bConnFd�Ƿ����Ѿ����ӵ�fd ��SocketConn �ӿڽ�ʧЧ
	bufferevent* AttachSocket(evutil_socket_t connfd, int options);
	void AttachSocketCb(bufferevent_data_cb readpfn, bufferevent_data_cb writepfn, bufferevent_event_cb evepfn, void* arg);
	void SocketDisConn();
	void SetCallEve(short event);
	int GetConnType();
	//���͵�������bufferevent�ӹ�
	bool Send(const char* buf, int len);
	int Read(char** buf, bufferevent* bev);
	virtual void Release();
protected:
	bufferevent* m_bev;
	int m_ConnType;
};
//�ͻ��� �����Ӷ���
class CBaseEventConnClient : public CEventBaseConn
{
public:
	CBaseEventConnClient();
	virtual~CBaseEventConnClient();
	//�ͻ�������
	int  SocketConn(const char* ip, int port);
};

