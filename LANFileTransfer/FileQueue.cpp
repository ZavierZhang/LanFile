#include "stdafx.h"
#include "FileQueue.h"


CFileQueue::CFileQueue()
{
}


CFileQueue::~CFileQueue()
{
	CAutoLock lock(m_LockWork);
	for (std::map<uint32_t, CFileRecv*>::iterator iter = m_mapWork.begin(); iter != m_mapWork.end(); iter++)
	{
		CFileRecv* pObj = iter->second;
		delete pObj;
	}
}
void CFileQueue::AddFile(uint32_t uKey, CFileWaitConnect& obj)
{
	CAutoLock lock(m_LockAll);
	m_mapAll[uKey] = obj;
	m_Link[obj.szSessionID] = uKey;
}
bool CFileQueue::AddWorkFile(uint32_t uKey, CFileExt& ext, std::string& guid)
{
	CFileRecv* pRecive = new CFileRecv;
	pRecive->SetFileExt(&ext);
	guid = pRecive->GetNextReciveGuid();
	pRecive->SetCurrentReciveFile(guid);
	if (m_mapAll.find(uKey) != m_mapAll.end())
	{
		std::string  basepath = m_mapAll[uKey].szRecvPath;
		pRecive->SetRecvBasePath(basepath);   //ÉèÖÃ»ù´¡Â·¾¶
	}
	else
	{
		return false;
	}
	if(!pRecive->Start(true))
	       return false;
	CAutoLock lock(m_LockWork);
	m_mapWork[uKey] = pRecive;
	return true;
}
CFileRecv* CFileQueue::GetWorkFileObj(uint32_t uKey)
{
	if (m_mapWork.find(uKey) != m_mapWork.end())
	{
		return m_mapWork[uKey];
	}
	return NULL;
}
uint32_t CFileQueue::GetConnKey(std::string& sessionid)
{
	if (m_Link.find(sessionid) != m_Link.end())
		return m_Link[sessionid];
	return 0;
}
std::string CFileQueue::GetConnSessionID(uint32_t uKey)
{
	std::string sessionid;
	if (m_mapAll.find(uKey) != m_mapAll.end())
	{
		sessionid = m_mapAll[uKey].szSessionID;
	}
	return sessionid;
}
std::map<std::string, uint32_t>& CFileQueue::GetAllLink()
{
	return m_Link;
}
void CFileQueue::DeleteFile(uint32_t uKey)
{
	CAutoLock lock(m_LockAll);
	if (m_mapAll.find(uKey) != m_mapAll.end())
	{
		std::string sessionid = m_mapAll[uKey].szSessionID;
		if (m_Link.find(sessionid) != m_Link.end())
		{
			m_Link.erase(sessionid);
		}
		m_mapAll.erase(uKey);
	}
}
void CFileQueue::DeleteWorkFile(uint32_t uKey)
{
	CAutoLock lock(m_LockWork);
	if (m_mapWork.find(uKey) != m_mapWork.end())
	{
		CFileRecv* pObj = m_mapWork[uKey];
		m_mapWork.erase(uKey);
		delete pObj;
	}
	DeleteFile(uKey);
}
void CFileQueue::StopFile(uint32_t uKey)
{
	CAutoLock lock(m_LockWork);
	if (m_mapWork.find(uKey) != m_mapWork.end())
	{
		CFileRecv* pObj = m_mapWork[uKey];
		pObj->SetRelease(true);
	}
}