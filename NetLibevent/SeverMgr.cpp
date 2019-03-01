#include "stdafx.h"
#include "SeverMgr.h"


CSeverMgr::CSeverMgr()
{
	m_pSever = NULL;
}


CSeverMgr::~CSeverMgr()
{
	if (NULL != m_pSever)
		delete m_pSever;
}
int CSeverMgr::Init(int port)
{
	if (NULL != m_pSever)
		return 0;
	m_pSever = new CSeverObj;
	m_pSever->Init(port);
	return 0;
}
void CSeverMgr::Reg(Util::LibEventSever::CEventSever* pSink)
{
	m_pSink = pSink;
}
void CSeverMgr::RegBeginSever(Util::LibEventSever::CBeginSeverRes* pSink)
{
	m_pBeginSink = pSink;
}
void CSeverMgr::BeginSever(bool bBlock)
{
	if (NULL == m_pSever)
		return;
	m_pSever->BeginSever(bBlock);
}
void CSeverMgr::SetFdCallBack(Netfd fd, bool bCbRead, bool bCbWrite)
{
	if (NULL == m_pSever)
		return;
	m_pSever->SetFdCallBack(fd,bCbRead,bCbWrite);
}
bool CSeverMgr::Send(Netfd fd, const char* buf, int len)
{
	if (NULL == m_pSever)
		return false;
	return m_pSever->Send(fd, buf, len);
}
void CSeverMgr::Stop(Netfd fd)
{
	if (NULL == m_pSever)
		return;
	m_pSever->Stop(fd);
}
void CSeverMgr::EndSever()
{
	if (NULL == m_pSever)
		return;
	m_pSever->EndSever();
}
void CSeverMgr::UnInit()
{
	if (NULL == m_pSever)
		return;
	m_pSever->UnInit();
}

void CSeverMgr::OnAccept(int fd)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnAccept(fd);
	}
}
void CSeverMgr::OnConnect(int fd, uint32_t uCode)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnFdConnet(fd,uCode);
	}
}
void CSeverMgr::OnWrite(int fd)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnFdWrite(fd);
	}
}
void CSeverMgr::OnRead(int fd, const char* buf, int len)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnFdRead(fd,buf,len);
	}
}
void CSeverMgr::OnBeginRes(bool bOk, int port)
{
	if (NULL != m_pBeginSink)
	{
		m_pBeginSink->OnBeginRes(bOk,port);
	}
}