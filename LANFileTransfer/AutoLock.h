#pragma once
//#include <shlwapi.h>
#include <mutex>

class LockHelper
{
public:
	LockHelper();
	virtual ~LockHelper();

public:
	void lock();
	void unLock();
private:
	//CRITICAL_SECTION _crit;
	std::mutex _crit;
};
class CAutoLock
{
public:
	explicit CAutoLock(LockHelper & lk) :_lock(lk) { _lock.lock(); }
	~CAutoLock() { _lock.unLock(); }
private:
	LockHelper & _lock;
};

