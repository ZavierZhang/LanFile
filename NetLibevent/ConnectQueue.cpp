#include "stdafx.h"
#include "ConnectQueue.h"

CConnectQueue::CConnectQueue()
{
}
CConnectQueue::~CConnectQueue()
{
}
void CConnectQueue::AddQueue(evutil_socket_t fd, CConnObj* pObj)
{
	m_mapQueue[fd] = pObj;
	m_mapRQueue[pObj] = fd;
}
CConnObj* CConnectQueue::GetConn(evutil_socket_t uKey)
{
	map<evutil_socket_t,CConnObj*>::iterator iter = m_mapQueue.find(uKey);
	if (iter != m_mapQueue.end())
		return iter->second;
	return NULL;
}
evutil_socket_t CConnectQueue::GetConn(CConnObj* bev)
{
	map<CConnObj*, evutil_socket_t>::iterator iter = m_mapRQueue.find(bev);
	if (iter != m_mapRQueue.end())
		return iter->second;
	return 0;
}
void CConnectQueue::DeleteQueue(evutil_socket_t uKey)
{
	map<evutil_socket_t, CConnObj*>::iterator iter = m_mapQueue.find(uKey);
	if (iter != m_mapQueue.end())
	{
		CConnObj* bev = iter->second;
		m_mapQueue.erase(iter);
		if (m_mapRQueue.find(bev) != m_mapRQueue.end())
		{
			m_mapRQueue.erase(bev);
		}
	}
}
void CConnectQueue::DeleteQueue(CConnObj* bev)
{
	map<CConnObj*,evutil_socket_t>::iterator iter = m_mapRQueue.find(bev);
	if (iter != m_mapRQueue.end())
	{
		evutil_socket_t uKey = iter->second;
		m_mapRQueue.erase(iter);
		if (m_mapQueue.find(uKey) != m_mapQueue.end())
		{
			m_mapQueue.erase(uKey);
		}
	}
}
map<evutil_socket_t, CConnObj*>& CConnectQueue::GetAllQueue()
{
	return m_mapQueue;
}