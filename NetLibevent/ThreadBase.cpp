#include "stdafx.h"
#include "ThreadBase.h"
#include "ThreadMgr.h"

CThreadBase::CThreadBase()
{
	m_uThreadID = 0;
	m_checkThread = nullptr;
}
CThreadBase::~CThreadBase()
{
}
bool CThreadBase::CreateThread(void* param)
{
	 m_uThreadID=CThreadMgr::GetObj().StartThread(CThreadBase::ThreadProc, this);
	 return true;
}
void  CThreadBase::ThreadProc(void* arg)
{
	CThreadBase* pThis = (CThreadBase*)arg;
	if (NULL == pThis)
		return ;
	pThis->Run_Thread(NULL);
	uint32_t ID = pThis->GetThreadID();
	pThis->OnFinish_Thread();
	CThreadMgr::GetObj().DeleteThread(ID);
	return ;
}
uint32_t CThreadBase::GetThreadID()
{
	return m_uThreadID;
}
void CThreadBase::OnFinish_Thread()
{
	m_uThreadID = 0;
}
void CThreadBase::DoExit(bool bForce)
{
	CThreadMgr::GetObj().WaitStop(m_uThreadID);
}
