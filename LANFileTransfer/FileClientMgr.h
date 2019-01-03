#pragma once
#include "UtilFileEvent.h"
#include "FileRecv.h"
#include "FileQueue.h"
//等待链接的文件信息

//客户端接受文件管理类
class CFileClientMgr : public Util::FILE_EVENT::CFileEvent
{
public:
	CFileClientMgr();
	~CFileClientMgr();
	//逻辑线程调用
	bool ReciveFile(const char*szSessionID, const char* szFilePath, const char* ip, int nPort);
	bool CancelRecive(const char*szSessionID);
	void StopAll();

	//文件工作线程调用接口  以下结构都是在文件线程中调用
	bool SendAgreeSaveFile(uint32_t uKey,std::string sessionid);
	bool SendFileCmd(uint32_t uKey, FileSign cmd, std::string& buf);
	//文件通讯命令
	virtual void OnFileCmd(FileSign cmd, uint32_t uKey, std::string& buf);
	virtual void OnFileSend(uint32_t uKey) {}
	virtual void OnFileRecive(uint32_t uKey, std::string& buf);
	//传输过程中 网络链接原因发送失败
	void OnFileError(uint32_t uKey, uint32_t uCode);
	void OnReciveSeverFileList(uint32_t uKey, std::string& buf);
	//事件发送文件时建立链接的返回结果，因为前面是有测试的 这边几乎不会失败
	void OnConnectRes(uint32_t uKey, uint32_t uCode);

	//通知回调函数
	void NotifyResult(std::string sessionid, uint32_t uCode);
	void NotifyProgress(const char* szSesssionID, uint64_t uPos, uint64_t uTotal);
private:
	CFileQueue   m_FileQueue;   //当前文件的工作队列
};

