#pragma once
#include "EventBase.h"
#include "EventBaseSever.h"
class CEventBaseSever :public CEventBase
{
public:
	CEventBaseSever();
	~CEventBaseSever();
	//����˼����ӿ�  ����listen ����  pfn listen �ص����� ptr userdata   port  ���������Ķ˿ں�
	evconnlistener* AttachListen(evconnlistener_cb pfn, void* ptr, unsigned flags, int port);
	int GetListenPort();
	virtual void Release();
private:
	evconnlistener* m_listen;
	int m_Port;   //���ŵĶ˿ں�  ����ⲿ���������Ƿ����Ѱ��ж˿ں�
};

