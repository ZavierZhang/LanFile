#pragma once
#include "FileClientMgr.h"
#include "UtilLanFileTrans.h"
//客户端管理对象
class ClientManagement
{
public:
	ClientManagement();
	~ClientManagement();
	bool ReciveFile(const char*szSessionID, const char* szFilePath,const char* ip, int nPort);
	void RegisterClientSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink);
	void OnPorgress(const char* szSesssionID,uint64_t uPos, uint64_t uTotal);
	void OnResult(const char* szSesssionID, uint32_t uCode);
	bool CancelRecive(const char*szSessionID);
	//退出
	void DoExit();
private:
	Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* m_pSink;
};

