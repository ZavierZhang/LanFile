#include "stdafx.h"
#include "ClientMgr.h"
#include "EventBase.h"

ClientMgr::ClientMgr()
{
}


ClientMgr::~ClientMgr()
{
}
void ClientMgr::Reg(Util::LibEventClient::EventClient::CEventClient* pSink)
{
	m_pSink = pSink;
}
Netfd ClientMgr::Connect(const char* ip, int port)
{
	ClientObj* pObj = new ClientObj;
	Netfd fd = pObj->Connect(ip, port);
	if (INVALID_SOCKET != fd)
	{
		m_ClientList[fd] = pObj;
	}
	else
	{
		delete pObj;
	}
	return fd;
}
ClientObj* ClientMgr::GetClient(Netfd fd)
{
	ClientObj* pObj = NULL;
	if (m_ClientList.find(fd) != m_ClientList.end())
	{
		pObj = m_ClientList[fd];
	}
	return pObj;
}
bool ClientMgr::Send(Netfd fd, const char* buf, int len)
{
	ClientObj* pObj = GetClient(fd);
	if (NULL == pObj)
	{
		return false;
	}
	return pObj->Send(buf, len);
}
void ClientMgr::DisConnect(Netfd fd)
{
	ClientObj* pObj = GetClient(fd);
	if (NULL == pObj)
	{
		return;
	}
	pObj->DisConnect(); 
}
void ClientMgr::OnConnect(Netfd fd,uint32_t uCode)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnConnect(uCode,fd);
	}
}
void ClientMgr::OnFdWrite(Netfd fd)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnFdWrite(fd);
	}
}
void ClientMgr::OnFdRead(Netfd fd, const char* buf, int len)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnFdRead(fd, buf, len);
	}
}
void ClientMgr::OnConnRelease(Netfd fd)
{
	if (m_ClientList.find(fd) != m_ClientList.end())
	{
		ClientObj* pObj = m_ClientList[fd];
		delete pObj;
		m_ClientList.erase(fd);
	}
}
void ClientMgr::DoExit()
{
	m_pSink = NULL;
	std::map<Netfd, ClientObj*> mapTemp = m_ClientList;
	for (std::map<Netfd, ClientObj*>::iterator iter = mapTemp.begin(); iter != mapTemp.end(); iter++)
	{
		ClientObj* pObj = iter->second;
		pObj->DoExit();  //确保链接和线程退出
	}
}