#pragma once
#include "FileSend.h"
#include "AutoLock.h"
//����˶���
class CFileSendQueue
{
public:
	CFileSendQueue();
	~CFileSendQueue();
	void AddFile(const char* szSessionID, CFileExt& obj);

	bool AddWorkFile(uint32_t uKey,std::string& sessionid,std::string& guid);

	void DeleteFile(std::string& sessionid);
	void DeleteWorkFile(uint32_t uKey);

	void StopFile(uint32_t uKey);

	CFileExt* GetFileExtObj(std::string& sessionid);

	CFileSend* GetWorkObj(uint32_t uKey);

	std::string GetWorkSessinID(uint32_t uKey);
	
	uint32_t GetConnKey(std::string& sessionid);

private:
	LockHelper m_LockList;
	//LockHelper m_LockWork;
	std::map<uint32_t, CFileSend*> m_List;  //��ǰ���ڷ��͵��ļ���Ϣ
	std::map<std::string, CFileExt> m_All;  //���е��ļ�������Ϣ
	std::map<std::string, uint32_t> m_AllConn;  //sessionid ��key��ӳ��
	std::map<uint32_t, std::string> m_RAllConn;
};

