#include "stdafx.h"
#include "SeverManagement.h"
#include "Singleton.h"
#include "UtilSever.h"

CSeverManagement::CSeverManagement()
{
	m_bStart = false;
}


CSeverManagement::~CSeverManagement()
{
	Singleton<CFileSeverMgr>::UnInstance();
	Util::LibEventSever::UnInit();
}
int CSeverManagement::Start(Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve* pSink)
{
	if (m_bStart)
		return -1;
	m_pSink = pSink;
	Util::LibEventSever::Init(0);
	Util::LibEventSever::BeginSever();
	Util::LibEventSever::RegBeginSever(this);
	m_bStart = true;
	return 0;
}
void CSeverManagement::Stop()
{
	m_bStart = false;
	Singleton<CFileSeverMgr>::Instance().StopAll();  //Í£Ö¹ËùÓÐÁ´½Ó
	Util::LibEventSever::RegBeginSever(NULL);
	Util::LibEventSever::EndSever();
	
}
void CSeverManagement::AddFile(const char*szSessionID, const char* szPath)
{
	Singleton<CFileSeverMgr>::Instance().AddFile(szSessionID, szPath);
}
bool CSeverManagement::CancelSend(const char*szSessionID)
{
	return Singleton<CFileSeverMgr>::Instance().CancelSend(szSessionID);
}
void CSeverManagement::RegisterSeverSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink)
{
	Singleton<CFileSeverMgr>::Instance().Init(pSink);
}
void CSeverManagement::OnBeginRes(bool bOk, int port)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnBeginSever(bOk, port);
	}
}