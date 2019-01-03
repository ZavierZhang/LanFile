#pragma once
#include "UtilSever.h"
#include "SeverObj.h"
class CSeverMgr
{
public:
	CSeverMgr();
	~CSeverMgr();
	int Init(int port);
	void Reg(Util::LibEventSever::CEventSever* pSink);
	void RegBeginSever(Util::LibEventSever::CBeginSeverRes* pSink);
	void BeginSever(bool bBlock = false);
	void SetFdCallBack(Netfd fd, bool bCbRead, bool bCbWrite);
	bool Send(Netfd fd, const char* buf, int len);
	void Stop(Netfd fd);
	void EndSever();
	void UnInit();

	//面向底层调用
	 void OnAccept(int fd);
	 void OnConnect(int fd, uint32_t uCode);
	 void OnWrite(int fd);
	 void OnRead(int fd, const char* buf, int len);
	 void OnBeginRes(bool bOk, int port);
private:
	CSeverObj* m_pSever;  //Sever 对象
	Util::LibEventSever::CEventSever* m_pSink;   //网络事件监听回调
	Util::LibEventSever::CBeginSeverRes* m_pBeginSink;  //启动事件回调
};

