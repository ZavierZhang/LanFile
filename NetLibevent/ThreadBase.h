#pragma once
#include <thread>
class CThreadBase
{
public:
	CThreadBase();
	~CThreadBase();
	bool CreateThread(void* param);
	uint32_t GetThreadID();
	virtual void Run_Thread(void* param) = 0;
	virtual void OnFinish_Thread();
	void DoExit(bool bForce);
private:
	static void  ThreadProc(void* arg);
private:
	size_t    m_uThreadID;
	struct ParamInfo
	{
		ParamInfo()
		{
			param = NULL;
			param = NULL;
		}
		void* param;
		void* pThis;
	};
	ParamInfo m_info;
	std::shared_ptr<std::thread> m_checkThread;
};

