#pragma once
#include "UtilConnTest.h"
class ConnTestMgr
{
public:
	ConnTestMgr();
	~ConnTestMgr();
	Netfd ConnTest(const char*ip, int port);
	void DoExit();
	void Reg(Util::LibEventConnTest::LibEventConnTestEve::CLibEventConnTestEve* pSink);
	void OnConn(Netfd fd, bool bOk);
private:
	Util::LibEventConnTest::LibEventConnTestEve::CLibEventConnTestEve* m_pSink;  //»Øµ÷Ö¸Õë
};

