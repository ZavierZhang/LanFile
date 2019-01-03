#pragma once
#include "FileRecv.h"
#include "AutoLock.h"
//文件队列
struct CFileWaitConnect
{
	std::string szRecvPath;   //接受文件的路径
	std::string szSessionID;
	std::string ip;
	int port;
	uint32_t uKey;
};
class CFileQueue
{
public:
	CFileQueue();
	~CFileQueue();
	void AddFile(uint32_t uKey, CFileWaitConnect& obj); 
	bool AddWorkFile(uint32_t uKey,CFileExt& ext,std::string& guid);

	void DeleteFile(uint32_t uKey);
	void DeleteWorkFile(uint32_t uKey);

	void StopFile(uint32_t uKey);

	CFileRecv* GetWorkFileObj(uint32_t uKey);
	uint32_t GetConnKey(std::string& sessionid);
	std::string GetConnSessionID(uint32_t uKey);
	std::map<std::string, uint32_t>& GetAllLink();

private:
	LockHelper m_LockAll;
	LockHelper m_LockWork;
	std::map<uint32_t, CFileRecv*> m_mapWork;  //当前正在有传输文件的链接
	std::map<uint32_t, CFileWaitConnect>m_mapAll;   //当前所有的链接数，包括正在链接的 未进行文件传输的
	std::map<std::string, uint32_t> m_Link;   //session id 对应的链接数
};

