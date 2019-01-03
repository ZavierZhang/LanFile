#include "stdafx.h"
#include "ClientObj.h"
#include "ClientMgr.h"
#include"Singleton.h"
ClientObj::ClientObj()
{
}


ClientObj::~ClientObj()
{
}
Netfd ClientObj::Connect(const char* ip, int port)
{
	CConnObj* pObj = new CConnObj(this);
	 Netfd fd=pObj->Init(ip, port);
	 if (INVALID_SOCKET == fd)
		 delete pObj;
	 m_pObj = pObj;
	 return fd;
}
bool ClientObj::Send(const char* buf, int len)
{
	if (NULL == m_pObj)
	{
		return false;
	}
	return m_pObj->Send(buf, len);
}
void ClientObj::DisConnect()
{
	if (NULL == m_pObj)
	{
		return ;
	}
	m_pObj->Stop();
}
//��ߵȴ��߳��˳� ������ֻ���Ƿ��ļ������߳��е���
void ClientObj::DoExit()
{
	DisConnect();  //�˳�����
	if (NULL == m_pObj)
	{
		return;
	}
	m_pObj->DoExit(false);   // �ȴ��߳��˳�
}
void ClientObj::OnConnect(evutil_socket_t fd, uint32_t uCode)
{
	Singleton<ClientMgr>::Instance().OnConnect(fd, uCode);
}
void ClientObj::OnWrite(evutil_socket_t fd)
{
	Singleton<ClientMgr>::Instance().OnFdWrite(fd);
}
void ClientObj::OnRead(evutil_socket_t fd, const char* buf, int len)
{
	Singleton<ClientMgr>::Instance().OnFdRead(fd, buf, len);
}
void ClientObj::OnFinishRelease(evutil_socket_t fd)
{
	Singleton<ClientMgr>::Instance().OnConnRelease(fd);
}
