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
//这边等待线程退出 理论上只能是非文件传输线程中调用
void ClientObj::DoExit()
{
	DisConnect();  //退出链接
	if (NULL == m_pObj)
	{
		return;
	}
	m_pObj->DoExit(false);   // 等待线程退出
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
