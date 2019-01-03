#include "stdafx.h"
#include "UtilSever.h"
#include "SeverMgr.h"
#include "Singleton.h"
int Util::LibEventSever::Init(int port)
{
	return Singleton<CSeverMgr>::Instance().Init(port);
}

void Util::LibEventSever::Reg(Util::LibEventSever::CEventSever* pSink)
{
	Singleton<CSeverMgr>::Instance().Reg(pSink);
}
void Util::LibEventSever::RegBeginSever(Util::LibEventSever::CBeginSeverRes* pSink)
{
	Singleton<CSeverMgr>::Instance().RegBeginSever(pSink);
}
void Util::LibEventSever::BeginSever(bool bBlock)
{
	Singleton<CSeverMgr>::Instance().BeginSever(bBlock);
}
bool Util::LibEventSever::Send(Netfd fd, const char* buf, int len)
{
  return Singleton<CSeverMgr>::Instance().Send(fd, buf, len);
}
void Util::LibEventSever::SetFdCallBack(Netfd fd, bool bCbRead, bool bCbWrite)
{
	return Singleton<CSeverMgr>::Instance().SetFdCallBack(fd, bCbRead, bCbWrite);
}
void Util::LibEventSever::Stop(Netfd fd)
{
	Singleton<CSeverMgr>::Instance().Stop(fd);
}
void Util::LibEventSever::EndSever()
{
	Singleton<CSeverMgr>::Instance().EndSever();
}
void Util::LibEventSever::UnInit()
{
	Singleton<CSeverMgr>::UnInstance();
}
