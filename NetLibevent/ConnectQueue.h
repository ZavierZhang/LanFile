#pragma once
#include "EventBase.h"
#include "ConnObj.h"
#include <map>
using namespace std;
//��������Ӷ���
class CConnectQueue
{
public:
	CConnectQueue();
	~CConnectQueue();
	void AddQueue(evutil_socket_t fd,CConnObj* pObj);
	CConnObj* GetConn(evutil_socket_t fd);
	evutil_socket_t GetConn(CConnObj* pObj);
	void DeleteQueue(evutil_socket_t fd);
	void DeleteQueue(CConnObj* pObj);
	map<evutil_socket_t, CConnObj*>& GetAllQueue();
private:
	map<evutil_socket_t, CConnObj*> m_mapQueue;     //bev  ��key ��һһ��Ӧ��
	map<CConnObj*, evutil_socket_t> m_mapRQueue;  //�����
};

