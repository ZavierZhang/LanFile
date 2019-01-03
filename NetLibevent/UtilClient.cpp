#include "stdafx.h"
#include "UtilClient.h"
#include "ClientMgr.h"
#include "Singleton.h"
void Util::LibEventClient::Reg(Util::LibEventClient::EventClient::CEventClient* pSink)
{
	Singleton<ClientMgr>::Instance().Reg(pSink);
}
Netfd Util::LibEventClient::Connect(const char* ip, int port)
{
	return Singleton<ClientMgr>::Instance().Connect(ip, port);
}
bool Util::LibEventClient::Send(Netfd fd, const char* buf, int len)
{
	return Singleton<ClientMgr>::Instance().Send(fd, buf, len);
}
void Util::LibEventClient::DisConnect(Netfd fd)
{
	Singleton<ClientMgr>::Instance().DisConnect(fd);
}
void Util::LibEventClient::DoExit()
{
	Singleton<ClientMgr>::Instance().DoExit();
	Singleton<ClientMgr>::UnInstance();
}