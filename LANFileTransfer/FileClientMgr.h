#pragma once
#include "UtilFileEvent.h"
#include "FileRecv.h"
#include "FileQueue.h"
//�ȴ����ӵ��ļ���Ϣ

//�ͻ��˽����ļ�������
class CFileClientMgr : public Util::FILE_EVENT::CFileEvent
{
public:
	CFileClientMgr();
	~CFileClientMgr();
	//�߼��̵߳���
	bool ReciveFile(const char*szSessionID, const char* szFilePath, const char* ip, int nPort);
	bool CancelRecive(const char*szSessionID);
	void StopAll();

	//�ļ������̵߳��ýӿ�  ���½ṹ�������ļ��߳��е���
	bool SendAgreeSaveFile(uint32_t uKey,std::string sessionid);
	bool SendFileCmd(uint32_t uKey, FileSign cmd, std::string& buf);
	//�ļ�ͨѶ����
	virtual void OnFileCmd(FileSign cmd, uint32_t uKey, std::string& buf);
	virtual void OnFileSend(uint32_t uKey) {}
	virtual void OnFileRecive(uint32_t uKey, std::string& buf);
	//��������� ��������ԭ����ʧ��
	void OnFileError(uint32_t uKey, uint32_t uCode);
	void OnReciveSeverFileList(uint32_t uKey, std::string& buf);
	//�¼������ļ�ʱ�������ӵķ��ؽ������Ϊǰ�����в��Ե� ��߼�������ʧ��
	void OnConnectRes(uint32_t uKey, uint32_t uCode);

	//֪ͨ�ص�����
	void NotifyResult(std::string sessionid, uint32_t uCode);
	void NotifyProgress(const char* szSesssionID, uint64_t uPos, uint64_t uTotal);
private:
	CFileQueue   m_FileQueue;   //��ǰ�ļ��Ĺ�������
};

