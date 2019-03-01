#pragma once
#include "UtilClient.h"
#include"ClientObj.h"
#include <map>
class ClientMgr
{
public:
	ClientMgr();
	~ClientMgr();
	 void Reg(Util::LibEventClient::EventClient::CEventClient* pSink);
	 Netfd Connect(const char* ip, int port);
	 bool Send(Netfd fd, const char* buf, int len);
	 void DisConnect(Netfd fd);
	 void DoExit();
	 //����ײ����
	 void OnConnect(Netfd fd,uint32_t uCode);
	 void OnFdWrite(Netfd fd);
	 void OnFdRead(Netfd fd, const char* buf, int len);


	 void OnConnRelease(Netfd fd);
protected:
	ClientObj* GetClient(Netfd fd);
	
private:
	std::map<Netfd, ClientObj*> m_ClientList;   //�ͻ������е����Ӷ���  
	Util::LibEventClient::EventClient::CEventClient* m_pSink;
};

