#include "stdafx.h"
#include "CheckNetMgr.h"


CCheckNetMgr::CCheckNetMgr()
{
	m_pSink = NULL;
}


CCheckNetMgr::~CCheckNetMgr()
{
	Util::LibEventConnTest::DoExit();
}
void CCheckNetMgr::Register(Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent* pSink)
{
	m_pSink = pSink;
	Util::LibEventConnTest::Reg(this);
}
void CCheckNetMgr::OnConnect(Netfd fd, bool bOk)
{
	std::map<uint32_t, CheckNetInfo>::iterator iter = m_mapCheckInfo.find(fd);
	if (iter == m_mapCheckInfo.end())
	{
		return;
	}
	if (bOk)
	{
		m_mapOK[iter->second.ip] = iter->second.port;
	}

	if (NULL != m_pSink)
	{
		m_pSink->OnCanLanFile(fd,iter->second.ip.c_str(), bOk);
	}
}
uint32_t CCheckNetMgr::CheckIP(const char* szIP, int nPort)
{
	if (nPort <= 0 || NULL == szIP)
		return 0;
	if (m_mapOK.find(szIP) != m_mapOK.end())
	{
		return 1;   //缓存检测可用
	}
	uint32_t uKey=Util::LibEventConnTest::ConnTest(szIP, nPort);
	CheckNetInfo info;
	info.ip = szIP;
	info.port = nPort;
	m_mapCheckInfo[uKey] = info;
	return uKey;    //等待结果
}
