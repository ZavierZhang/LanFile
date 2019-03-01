#include "stdafx.h"
#include "TimeOut.h"
#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

#define  TIME_OUT  1   //进度投递间隔1S 一次

CTimeOut::CTimeOut()
{
}


CTimeOut::~CTimeOut()
{
}

void CTimeOut::Init()
{
	m_dwLastTime = time(0);
}
void CTimeOut::Reset()
{
	m_dwLastTime = 0;
}
bool CTimeOut::IsTimeOut()
{
	if ((time(0) - m_dwLastTime) > TIME_OUT)
		return true;
	return false;
}
void CTimeOut::Update()
{
	m_dwLastTime = time(0);
}
