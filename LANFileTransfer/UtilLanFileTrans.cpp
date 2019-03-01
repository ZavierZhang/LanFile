#include "stdafx.h"
#include "UtilLanFileTrans.h"
#include "SeverManagement.h"
#include "ClientManagement.h"
#include "CheckNetMgr.h"
#include"Singleton.h"
#include "UtilTool.h"
#ifdef WIN32
#include "log4z.h"
#endif

void Util::LAN_FILE::InitModule()
{
#ifdef WIN32
	log4z::ILog4zManager::getInstance()->start();
#endif
}
void Util::LAN_FILE::UnInitModule()
{
#ifdef WIN32
	log4z::ILog4zManager::getInstance()->stop();
#endif
}

int Util::LAN_FILE::Sever::Init(Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve* pSink)
{
	
	return Singleton<CSeverManagement>::Instance().Start(pSink);
}
void Util::LAN_FILE::Sever::UnInit()
{
	 Singleton<CSeverManagement>::Instance().Stop();
	 Singleton<CSeverManagement>::UnInstance();
}

//标识文件操作的filekey
void Util::LAN_FILE::Sever::SendFile(const char*szSessionID, const char* szPath)
{
	Singleton<CSeverManagement>::Instance().AddFile(szSessionID, szPath);
}
bool Util::LAN_FILE::Sever::CancelSend(const char*szSessionID)
{
	return Singleton<CSeverManagement>::Instance().CancelSend(szSessionID);
}
void Util::LAN_FILE::Sever::RegisterSeverSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink)
{
	Singleton<CSeverManagement>::Instance().RegisterSeverSink(pSink);
}
bool Util::LAN_FILE::Client::ReciveFile(const char*szSessionID, const char* szPath, const char* ip, int nPort)
{
	return Singleton<ClientManagement>::Instance().ReciveFile(szSessionID, szPath,ip,nPort);
}
bool Util::LAN_FILE::Client::CancelRecive(const char*szSessionID)
{
	return Singleton<ClientManagement>::Instance().CancelRecive(szSessionID);
}
uint32_t Util::LAN_FILE::Client::CanLanFile(const char* ip, int nPort)
{
	return Singleton<CCheckNetMgr>::Instance().CheckIP(ip, nPort);
}

void Util::LAN_FILE::Client::RegisterClientSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink)
{
	Singleton<ClientManagement>::Instance().RegisterClientSink(pSink);
}
void Util::LAN_FILE::Client::RegisterCheckNetSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent* pSink)
{
	Singleton<CCheckNetMgr>::Instance().Register(pSink);
}
void Util::LAN_FILE::Client::DoClientExit()
{
	Singleton<CCheckNetMgr>::UnInstance(); //退出测试管理类
	Singleton<ClientManagement>::Instance().DoExit();
	Singleton<ClientManagement>::UnInstance();
}


std::string  Util::OneGuid::GetOneGuid()
{
	 return Util::Tool::GetGuid();
}