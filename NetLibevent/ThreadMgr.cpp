#include "stdafx.h"
#include "ThreadMgr.h"
#include <sstream>

CThreadMgr::CThreadMgr()
{
}
CThreadMgr& CThreadMgr::GetObj()
{
	static CThreadMgr obj;
	return obj;
}
CThreadMgr::~CThreadMgr()
{
	for (std::map<size_t, std::thread*>::iterator iter = m_mapFinshThread.begin(); iter != m_mapFinshThread.end(); iter++)
	{
		std::thread*  pThread = iter->second;
		if(pThread->joinable())
		     pThread->join();
		delete pThread;
	}
}
size_t CThreadMgr::StartThread(ThreadBaseCallBack pfnThread, void* arg)
{
	m_mutex.lock();
	std::thread*  pThread = new std::thread(pfnThread, arg);
	pThread->joinable();
	size_t ID = std::hash<std::thread::id>()(pThread->get_id());
	m_mapThread[ID] = pThread;
	m_mutex.unlock();
	return ID;
}
void CThreadMgr::DeleteThread(size_t id)
{
	m_mutex.lock();
	if (m_mapThread.find(id) != m_mapThread.end())
	{
		std::thread*  pThread = m_mapThread[id];
		m_mapThread.erase(id);
		m_mapFinshThread[id] = pThread;
	}
	m_mutex.unlock();
}

void CThreadMgr::WaitStop(size_t id)
{
	if (m_mapThread.find(id) != m_mapThread.end())
	{
		std::thread*  pThread = m_mapThread[id];
		if (NULL != pThread)
		{
			if(pThread->joinable())
				pThread->join();
		}	
	}
}
