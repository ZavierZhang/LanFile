#pragma once
//#include "TcpSeverListen.h"
#include "FileSeverMgr.h"
#include "UtilLanFileTrans.h"
#include "UtilSever.h"
//本机的服务端  这边本机服务端只会开一个监听连接CSeverManagement 应该为单例模式
class CSeverManagement :public Util::LibEventSever::CBeginSeverRes
{
public:
	CSeverManagement();
	~CSeverManagement();
	//启动服务端
	int Start(Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve* pSink);
	void Stop();  //停止服务端工作

	void AddFile(const char*szGuiID, const char* szPath);
	bool CancelSend(const char*szSessionID);
	void OnBeginRes(bool bOk, int port);
	void RegisterSeverSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink);
private:
	bool m_bStart;
	Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve* m_pSink;
};

