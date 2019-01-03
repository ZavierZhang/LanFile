#pragma once
class CThreadBase
{
public:
	CThreadBase();
	~CThreadBase();
	bool CreateThread();
	UINT GetThreadID();
	virtual void Run_Thread() = 0;
	virtual void OnFinish_Thread() = 0;
private:
	static UINT WINAPI  ThreadProc(void* pVoid);
private:
	UINT    m_uThreadID;
	HANDLE m_hThreadHandle;  
};

