#pragma once
#include <stdint.h>

class CTimeOut
{
public:
	CTimeOut();
	~CTimeOut();
	void Init();
	void Reset();
	bool IsTimeOut();
	void Update();
private:
	uint64_t m_dwLastTime;
};
