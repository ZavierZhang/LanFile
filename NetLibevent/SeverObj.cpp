#include "stdafx.h"
#include "SeverObj.h"
#include "SeverMgr.h"
#include "Singleton.h"
CSeverObj::CSeverObj()
{
	m_plisten = NULL;
	m_pConnQueue = NULL;
}


CSeverObj::~CSeverObj()
{
	if (NULL != m_pConnQueue)
		delete m_pConnQueue;
}
int CSeverObj::Init(int port)
{
	m_port = port;
	return 0;
}
void CSeverObj::BeginSever(bool bBlock)
{
	m_plisten = new CListenObj;
	m_plisten->Reg(this);
	m_pConnQueue = new CConnectQueue;
	m_plisten->Start(m_port);
}
void CSeverObj::SetFdCallBack(Netfd fd, bool bCbRead, bool bCbWrite)
{
	CConnObj* pObj = m_pConnQueue->GetConn(fd);
	if (NULL == pObj)
		return ;
	pObj->SetFdCallBack(fd, bCbRead, bCbWrite);
}
void CSeverObj::OnAccept(evutil_socket_t fd)
{
	CConnObj* pObj = new CConnObj(this);
	pObj->Init(fd);
	m_pConnQueue->AddQueue(fd, pObj);
	Singleton<CSeverMgr>::Instance().OnAccept(fd);
}
void CSeverObj::OnListen(bool bOK, int port)
{
	Singleton<CSeverMgr>::Instance().OnBeginRes(bOK, port);
}
void CSeverObj::OnConnect(evutil_socket_t fd, uint32_t uCode)
{
	m_pConnQueue->DeleteQueue(fd);   //删除连接队列
	Singleton<CSeverMgr>::Instance().OnConnect(fd, uCode);
}
void CSeverObj::OnWrite(evutil_socket_t fd)
{
	Singleton<CSeverMgr>::Instance().OnWrite(fd);
}
void CSeverObj::OnRead(evutil_socket_t fd, const char* buf, int len)
{
	Singleton<CSeverMgr>::Instance().OnRead(fd,buf,len);
}
bool CSeverObj::Send(Netfd fd, const char* buf, int len)
{
	CConnObj* pObj= m_pConnQueue->GetConn(fd);
	if (NULL == pObj)
		return false;
	return pObj->Send(buf, len);
}
void CSeverObj::Stop(Netfd fd)
{
	CConnObj* pObj = m_pConnQueue->GetConn(fd);
	if (NULL == pObj)
		return ;
	pObj->Stop();
}
void CSeverObj::EndSever()
{
	//关闭所有链接
	if (NULL != m_pConnQueue)
	{
		map<evutil_socket_t, CConnObj* > maptemp = m_pConnQueue->GetAllQueue();
		for (map<evutil_socket_t, CConnObj* >::iterator iter = maptemp.begin(); iter != maptemp.end(); iter++)
		{
			CConnObj* pObj = iter->second;
			if (NULL != pObj)
				pObj->Stop();
		}
	}
	if (NULL != m_plisten)
	{
		m_plisten->Stop();  //关闭监听端口
		m_plisten->DoExit(false);
	}
	     
}

void CSeverObj::OnFinishRelease(evutil_socket_t fd)
{
	if (NULL != m_pConnQueue)
		m_pConnQueue->DeleteQueue(fd);
}

void CSeverObj::UnInit()
{

}
