#pragma once
#include "UtilFileEvent.h"
#include "FileSend.h"
#include <map>
#include <string>
//#include "json.h"
#include "UtilLanFileTrans.h"
#include "FileSendQueue.h"
class CFileSeverMgr :public Util::FILE_EVENT::CFileEvent
{
public:
	CFileSeverMgr();
	~CFileSeverMgr();
	void Init(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink);
	void AddFile(const char*szSessionID, const char* szPath);
	void StopAll();
	bool CancelSend(const char*szSessionID);
	virtual void OnFileCmd(FileSign cmd, uint32_t uKey, std::string& buf);
	void OnAgreeFile(uint32_t uKey, std::string& buf);
	//�ͻ���������ĳһ���ļ�
	void OnReqSendFile(uint32_t uKey, std::string& buf);
	//�ͻ��˷������� �ļ���������
	void OnCompleteSendFile(uint32_t uKey, std::string& buf);
	//ĳһ���ļ� ���ճɹ���
	void OnFileSendSuccess(uint32_t uKey, std::string& buf);
	//��Ӧ�ĵײ�����key ���Է���������
	virtual void OnFileSend(uint32_t uKey);
	virtual void OnFileRecive(uint32_t uKey, std::string& buf);
	virtual void OnFileError(uint32_t uKey, uint32_t uCode);
	bool DoSendFile(uint32_t uKey);
	
	void NotifypProgress(std::string& sessionid, uint64_t uPos, uint64_t uTotal);
	void NotifyResult(std::string& sessionid, uint32_t uCode);
protected:
	//����˷����ļ���Ϣ�б� ���ͻ���
	bool SeverSendFileList(uint32_t uKey,CFileExt& ext, std::string& sessionid);
private:
	CFileSendQueue m_Queue;
	Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* m_pSink;
};

