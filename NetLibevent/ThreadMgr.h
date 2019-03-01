#pragma once
#include <thread>
#include <map>
#include <mutex>

//这边对象程序退出时释放所有线程对象

typedef void(*ThreadBaseCallBack)(void* arg);

class CThreadMgr
{
public:
	static CThreadMgr& GetObj();
	~CThreadMgr();
	size_t StartThread(ThreadBaseCallBack pfnThread,void* arg);
	void DeleteThread(size_t id);

	//等待线程退出
	void WaitStop(size_t id);
protected:
	CThreadMgr();
private:
	std::map<size_t,std::thread*> m_mapThread;
	std::map<size_t, std::thread*> m_mapFinshThread;
	std::mutex m_mutex;
};

