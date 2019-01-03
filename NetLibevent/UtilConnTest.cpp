#include "stdafx.h"
#include "UtilConnTest.h"
#include "ConnTestMgr.h"
#include "Singleton.h"

Netfd Util::LibEventConnTest::ConnTest(const char*ip, int port)
{
	return Singleton<ConnTestMgr>::Instance().ConnTest(ip, port);
}

void Util::LibEventConnTest::Reg(Util::LibEventConnTest::LibEventConnTestEve::CLibEventConnTestEve* pSink)
{
	return Singleton<ConnTestMgr>::Instance().Reg(pSink);
}
void Util::LibEventConnTest::DoExit()
{
	Singleton<ConnTestMgr>::UnInstance();
}