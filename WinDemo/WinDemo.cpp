// WinDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <codecvt>
#ifdef WIN32
#include <WinSock2.h>
#include <time.h>

#else
#include <time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <locale>
#endif
#include <stdio.h>
#include <iostream>
#include <string>
#include "UtilLanFileTrans.h"

using namespace std;

class CTest : public Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent, public Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent
{
public:
	virtual void OnCanLanFile(uint32_t uKey, const char* ip, bool bCan)
	{
		std::cout << "ip:" << ip << "   res=" << bCan << "\n";
		if (bCan)
		{
			cout << "������ɹ��ɷ����ļ�" << endl;
			cout << "�������ļ�·��" << endl;
		}
	}
	virtual void OnPorgress(const char* szSessionID, uint64_t uPos, uint64_t uTotal);
	virtual void OnResult(const char* szSessionID, uint32_t uCode);

};
uint32_t g_start = 0;
void CTest::OnPorgress(const char* szSessionID, uint64_t uPos, uint64_t uTotal)
{
	std::cout << "sessionid:" << szSessionID << "  pos: " << uPos << " total: " << uTotal << endl;
}

void CTest::OnResult(const char* szSessionID, uint32_t uCode)
{
	std::cout << "sessionid:" << szSessionID << "  res=" << uCode << endl;
	uint64_t uSpend = time(0) - g_start;
	cout << "spend time:" << uSpend << endl;
	cout << "��1 ������6�˳���" << endl;
}


class CSeverBegin :public Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve
{
	virtual void OnBeginSever(bool bOK, int Port)
	{
		std::cout << endl << "sever start res:" << bOK << " port=" << Port << "\n";
	}
};

bool GetLocalIP(std::string& strIP)
{

	char hostName[256];
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
#else
#endif
	if (!gethostname(hostName, sizeof(hostName)))
	{
		hostent *host = gethostbyname(hostName);
		if (host != NULL)
		{
			strIP = inet_ntoa(*(struct in_addr*)*host->h_addr_list);
			return true;
		}
	}

	return false;
}



void SeverContinue()
{
	cout << "--------------------------------------�����--------------------------------------" << endl;
	cout << "����sessionid��" << endl;
	string sessionid, path;
	cin >> sessionid;
	cout << "���뷢�͵��ļ�Ŀ¼��" << endl;
	cin >> path;
	Util::LAN_FILE::Sever::SendFile(sessionid.c_str(), path.c_str());
	cout << "����ļ��ɹ����ȴ��ͻ��˽���" << endl;
}

void ClientContinue()
{
	cout << "--------------------------------------�ͻ���--------------------------------------" << endl;
	string sessionid, path;
	cout << "����sessionid��" << endl;
	cin >> sessionid;
	cout << "��������IP:" << endl;
	std::string ip;
	cin >> ip;
	int port = 0;
	cout << "�������˿��Ŷ˿�:" << endl;
	cin >> port;
	cout << "���ڼ���Ƿ�ɷ����ļ�..." << endl;
	Util::LAN_FILE::Client::CanLanFile(ip.c_str(), port);
	string path1;
	cin >> path1;
	g_start = time(0);

	Util::LAN_FILE::Client::ReciveFile(sessionid.c_str(), path1.c_str(),ip.c_str(),port);
}

void ShowMain()
{
	int a = 0;
	cout << "1: �����   2���ͻ���"<< endl;
	
	if (cin >> a)
	{
		if (1 == a)
		{
			SeverContinue();
		}
		else if (2 == a)
		{
			ClientContinue();
		}
	}
}

int main(int argc, const char * argv[])
{
	// insert code here...

	cout << "ע�⣺û��ת����·������Ӣ��" << endl;

	Util::LAN_FILE::InitModule();

	std::string sz = Util::OneGuid::GetOneGuid();

	std::string ip;
	GetLocalIP(ip);
	std::cout << "������IP��ַ��" << ip;
	std::cout << endl;
	CSeverBegin sever;
	CTest EVE;
	Util::LAN_FILE::Client::RegisterCheckNetSink(&EVE);
	Util::LAN_FILE::Client::RegisterClientSink(&EVE);
	Util::LAN_FILE::Sever::RegisterSeverSink(&EVE);
	Util::LAN_FILE::Sever::Init(&sever);   //��ʼ��

	ShowMain();

	int b = 1;
	while (std::cin >> b)
	{
		if (6== b)
		{
			break;
		}
		if (1 == b)
		{
			ShowMain();
		}
	}
	Util::LAN_FILE::Sever::UnInit();
	Util::LAN_FILE::UnInitModule();
	return 0;
}
