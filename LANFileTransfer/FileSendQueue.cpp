#include "stdafx.h"
#include "FileSendQueue.h"


CFileSendQueue::CFileSendQueue()
{
}


CFileSendQueue::~CFileSendQueue()
{
	for (std::map<uint32_t, CFileSend*>::iterator iter = m_List.begin(); iter != m_List.end(); iter++)
	{
		CFileSend* pObj = iter->second;
		delete pObj;
	}
}
void CFileSendQueue::AddFile(const char* szSessionID, CFileExt& obj)
{
	std::string id = szSessionID;
	m_All[id] = obj;
}
bool CFileSendQueue::AddWorkFile(uint32_t uKey, std::string& sessionid, std::string& guid)
{
	CFileExt* pFileExt = GetFileExtObj(sessionid);
	if (NULL == pFileExt)
	{
		return false;  //请求的文件 服务端没有记录 异常了？？？
	}

	CAutoLock lock(m_LockList);   //Lock
	CFileSend* pFileSend = NULL;
	if (m_List.find(uKey) != m_List.end())
	{
		pFileSend = m_List[uKey];       //这边添加工作文件的时候 如果在list中找到了 说明 当前文件其实是个目录 发送对象一次操作只会对应一个
	}
	else
	{
		pFileSend = new CFileSend;
		m_List[uKey] = pFileSend;   //加到发送队列
		m_AllConn[sessionid] = uKey;
		m_RAllConn[uKey] = sessionid;
	}
	pFileSend->SetFileFlag(guid, true);
	pFileSend->SetFileExt(pFileExt);
	pFileSend->SetCurrentSendFile(guid);  //设置当前需要传输的文件
	return pFileSend->Start(true);
}

void CFileSendQueue::DeleteFile(std::string& sessionid)
{
	if (m_All.find(sessionid) != m_All.end())
		m_All.erase(sessionid);
}
CFileExt* CFileSendQueue::GetFileExtObj(std::string& sessionid)
{
	if (m_All.find(sessionid) != m_All.end())
		return &m_All[sessionid];
	return NULL;
}
CFileSend* CFileSendQueue::GetWorkObj(uint32_t uKey)
{
	if (m_List.find(uKey) != m_List.end())
		return m_List[uKey];
	return NULL;
}
std::string CFileSendQueue::GetWorkSessinID(uint32_t uKey)
{
	std::string sessionid;
	if (m_RAllConn.find(uKey) != m_RAllConn.end())
		sessionid = m_RAllConn[uKey];
	return sessionid;
}
uint32_t CFileSendQueue::GetConnKey(std::string& sessionid)
{
	if (m_AllConn.find(sessionid) != m_AllConn.end())
		return m_AllConn[sessionid];
	return 0;
}
void CFileSendQueue::StopFile(uint32_t uKey)
{
	if (m_List.find(uKey) != m_List.end())
	{
		CFileSend* pObj=m_List[uKey];
		pObj->SetRelease(true);
	}
}
void CFileSendQueue::DeleteWorkFile(uint32_t uKey)
{
	CAutoLock lock(m_LockList);   //Lock
	if (m_List.find(uKey) != m_List.end())
	{
		CFileSend* pSend = m_List[uKey];
		std::string sessionid = pSend->GetFileExt()->GetSessionID();
		DeleteFile(sessionid);
		m_List.erase(uKey);
		delete pSend;
		if (m_AllConn.find(sessionid) != m_AllConn.end())
			m_AllConn.erase(sessionid);
		if (m_RAllConn.find(uKey) != m_RAllConn.end())
			m_RAllConn.erase(uKey);
	}
}