#include "stdafx.h"
#include "ThreadBase.h"
#include <process.h>

CThreadBase::CThreadBase()
{
	m_uThreadID = 0;
	m_hThreadHandle = NULL;
}
CThreadBase::~CThreadBase()
{
}
bool CThreadBase::CreateThread()
{
	m_hThreadHandle=(HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, 0, &m_uThreadID);
	if (NULL == m_hThreadHandle || 0 == m_uThreadID)
	{
		return false;
	}
	return true;
}

UINT WINAPI  CThreadBase::ThreadProc(void* pVoid)
{
	CThreadBase* pThis = (CThreadBase*)pVoid;
	if (NULL == pThis)
		return 0;
	pThis->Run_Thread();
	pThis->OnFinish_Thread();
	return 0;
}
UINT CThreadBase::GetThreadID()
{
	return m_uThreadID;
}
