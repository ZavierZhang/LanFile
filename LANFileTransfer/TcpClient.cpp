#include "stdafx.h"
#include "TcpClient.h"
#include "Singleton.h"
#include "ClientPakcageMgr.h"
CTcpClient::CTcpClient()
{
	m_nFlag = 0;
	m_fd = 0;
}


CTcpClient::~CTcpClient()
{
	if(0!=m_fd)
		Util::LibEventClient::DisConnect(m_fd);
}
bool CTcpClient::Connect(const char* ip, int port,uint32_t& uKey)
{
	m_fd=Util::LibEventClient::Connect(ip, port);
	uKey = m_fd;
	return true;
}
void CTcpClient::Close()
{
	Util::LibEventClient::DisConnect(m_fd);
	m_fd = 0;
}
void CTcpClient::SetFlag(int nFlag)
{
	m_nFlag = nFlag;
}
int CTcpClient::GetFlag()
{
	return m_nFlag;
}
bool CTcpClient::Send(std::string& buf)
{
	return Util::LibEventClient::Send(m_fd,buf.c_str(),buf.length());
}



