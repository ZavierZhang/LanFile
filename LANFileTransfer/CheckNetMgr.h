#pragma once
//#include "CheckNet.h"
#include "UtilLanFileTrans.h"
#include "UtilConnTest.h"
#include <string>
#include<map>
struct CheckNetInfo
{
	std::string sessionid;
	std::string ip;
	int port;
};

class CCheckNetMgr : public Util::LibEventConnTest::LibEventConnTestEve::CLibEventConnTestEve
{
public:
	CCheckNetMgr();
	~CCheckNetMgr();
	uint32_t CheckIP(const char* szIP, int nPort);
	void Register(Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent* pSink);
	void OnConnect(Netfd fd, bool bOk);
private:
	std::map<std::string, int> m_mapOK;  //已探测可用的IP和端口
	std::map<uint32_t, CheckNetInfo> m_mapCheckInfo;
	Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent* m_pSink;
};

