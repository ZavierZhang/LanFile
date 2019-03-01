// WinDemo.cpp : 定义控制台应用程序的入口点。
//

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
#endif
#include <stdio.h>
#include <iostream>
#include <string>
#include "UtilLanFileTrans.h"

using namespace std;

class CTest : public Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent, public Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent
{
public:
	virtual void OnCanLanFile(const char* ip, bool bCan)
	{
		std::cout << "ip:" << ip << "   res=" << bCan << "\n";
		if (bCan)
		{
			cout << "检测结果成功可发送文件" << endl;
			cout << "请输入文件路径" << endl;
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
	cout << "按1 继续：6退出：" << endl;
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
	cout << "--------------------------------------服务端--------------------------------------" << endl;
	cout << "输入sessionid：" << endl;
	string sessionid, path;
	cin >> sessionid;
	cout << "输入发送的文件目录：" << endl;
	cin >> path;
	Util::LAN_FILE::Sever::SendFile(sessionid.c_str(), path.c_str());
	cout << "添加文件成功，等待客户端接受" << endl;
}

void ClientContinue()
{
	cout << "--------------------------------------客户端--------------------------------------" << endl;
	string sessionid, path;
	cout << "输入sessionid：" << endl;
	cin >> sessionid;
	cout << "输入服务端IP:" << endl;
	std::string ip;
	cin >> ip;
	int port = 0;
	cout << "输入服务端开放端口:" << endl;
	cin >> port;
	cout << "正在检测是否可发送文件..." << endl;
	Util::LAN_FILE::Client::CanLanFile(sessionid.c_str(), ip.c_str(), port);
	string path1;
	cin >> path1;
	g_start = time(0);
	Util::LAN_FILE::Client::ReciveFile(sessionid.c_str(), path1.c_str());
}

void ShowMain()
{
	int a = 0;
	cout << "1: 服务端                2：客户端" << endl;
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

	cout << "注意：没有转码请路径输入英文" << endl;

	Util::LAN_FILE::InitModule();

	std::string ip;
	GetLocalIP(ip);
	std::cout << "本机的IP地址：" << ip;
	std::cout << endl;
	CSeverBegin sever;
	CTest EVE;
	Util::LAN_FILE::Client::RegisterCheckNetSink(&EVE);
	Util::LAN_FILE::Client::RegisterClientSink(&EVE);
	Util::LAN_FILE::Sever::RegisterSeverSink(&EVE);
	Util::LAN_FILE::Sever::Init(&sever);   //初始化

	ShowMain();

	int b = 1;
	while (std::cin >> b)
	{
		if (6 == b)
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
