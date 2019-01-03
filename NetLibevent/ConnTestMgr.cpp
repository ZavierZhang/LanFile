#include "stdafx.h"
#include "ConnTestMgr.h"
#include "ConnTestObj.h"

ConnTestMgr::ConnTestMgr()
{
}


ConnTestMgr::~ConnTestMgr()
{

}
Netfd ConnTestMgr::ConnTest(const char*ip, int port)
{
	CConnTestObj* pObj = new CConnTestObj;
	return pObj->ConnTest(ip, port);
}
void ConnTestMgr::DoExit()
{

}
void ConnTestMgr::Reg(Util::LibEventConnTest::LibEventConnTestEve::CLibEventConnTestEve* pSink)
{
	m_pSink = pSink;
}
void ConnTestMgr::OnConn(Netfd fd, bool bOk)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnConnect(fd, bOk);
	}
}