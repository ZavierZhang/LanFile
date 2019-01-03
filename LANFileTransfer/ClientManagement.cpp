#include "stdafx.h"
#include "ClientManagement.h"
#include "Singleton.h"
ClientManagement::ClientManagement()
{
	m_pSink = NULL;
}


ClientManagement::~ClientManagement()
{
}
bool ClientManagement::ReciveFile(const char*szSessionID, const char* szFilePath,const char* ip, int nPort)
{
	return Singleton<CFileClientMgr>::Instance().ReciveFile(szSessionID, szFilePath,ip,nPort);
}
void ClientManagement::RegisterClientSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink)
{
	m_pSink = pSink;
}
void ClientManagement::OnPorgress(const char* szSesssionID,uint64_t uPos, uint64_t uTotal)
{
	if (NULL != m_pSink)
		m_pSink->OnPorgress(szSesssionID,uPos,uTotal);
}
void ClientManagement::OnResult(const char* szSesssionID, uint32_t uCode)
{
	if (NULL != m_pSink)
		m_pSink->OnResult(szSesssionID, uCode);
}
bool ClientManagement::CancelRecive(const char*szSessionID)
{
	return Singleton<CFileClientMgr>::Instance().CancelRecive(szSessionID);
}
void ClientManagement::DoExit()
{
	Singleton<CFileClientMgr>::Instance().StopAll();
	Singleton<CFileClientMgr>::UnInstance();
}