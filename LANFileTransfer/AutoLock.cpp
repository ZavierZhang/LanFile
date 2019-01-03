#include "stdafx.h"
#include "AutoLock.h"
//#pragma comment(lib, "shlwapi")
LockHelper::LockHelper()
{
	//InitializeCriticalSection(&_crit);
}
LockHelper::~LockHelper()
{
	//DeleteCriticalSection(&_crit);
}

void LockHelper::lock()
{
	//EnterCriticalSection(&_crit);
	_crit.lock();
}
void LockHelper::unLock()
{
	//LeaveCriticalSection(&_crit);
	_crit.unlock();
}
