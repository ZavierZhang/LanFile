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
		return false;  //������ļ� �����û�м�¼ �쳣�ˣ�����
	}

	CAutoLock lock(m_LockList);   //Lock
	CFileSend* pFileSend = NULL;
	if (m_List.find(uKey) != m_List.end())
	{
		pFileSend = m_List[uKey];       //�����ӹ����ļ���ʱ�� �����list���ҵ��� ˵�� ��ǰ�ļ���ʵ�Ǹ�Ŀ¼ ���Ͷ���һ�β���ֻ���Ӧһ��
	}
	else
	{
		pFileSend = new CFileSend;
		m_List[uKey] = pFileSend;   //�ӵ����Ͷ���
		m_AllConn[sessionid] = uKey;
		m_RAllConn[uKey] = sessionid;
	}
	pFileSend->SetFileFlag(guid, true);
	pFileSend->SetFileExt(pFileExt);
	pFileSend->SetCurrentSendFile(guid);  //���õ�ǰ��Ҫ������ļ�
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