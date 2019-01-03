#pragma once
#include <thread>
#include <map>
#include <mutex>

//��߶�������˳�ʱ�ͷ������̶߳���

typedef void(*ThreadBaseCallBack)(void* arg);

class CThreadMgr
{
public:
	static CThreadMgr& GetObj();
	~CThreadMgr();
	size_t StartThread(ThreadBaseCallBack pfnThread,void* arg);
	void DeleteThread(size_t id);

	//�ȴ��߳��˳�
	void WaitStop(size_t id);
protected:
	CThreadMgr();
private:
	std::map<size_t,std::thread*> m_mapThread;
	std::map<size_t, std::thread*> m_mapFinshThread;
	std::mutex m_mutex;
};

