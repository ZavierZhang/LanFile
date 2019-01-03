#include "stdafx.h"
#include "ClientPakcageMgr.h"
#include "FileClientMgr.h"
#include "Singleton.h"
CClientPakcageMgr::CClientPakcageMgr()
{
}


CClientPakcageMgr::~CClientPakcageMgr()
{
	Util::LibEventClient::Reg(NULL);
	Util::LibEventClient::DoExit();
}
void CClientPakcageMgr::RegisterSink(Util::FILE_EVENT::CFileEvent* pSink)
{
	m_pSink = pSink;
	Util::LibEventClient::Reg(this);
}
void  CClientPakcageMgr::Connect(const char*ip, int port, uint32_t& uKey)
{
	CTcpClient* pClient = new CTcpClient;
	uint32_t uKey1 = 0;
	if (!pClient->Connect(ip, port, uKey1))
		return;
	uKey = uKey1;
	m_Client[uKey] = pClient;
}
void CClientPakcageMgr::DisConnect(uint32_t& ukey)
{
	if (m_Client.find(ukey) == m_Client.end())
	{
		////assert(false);
		return ;
	}
	CTcpClient* pClient = m_Client[ukey];
	m_Client.erase(ukey);
	if (NULL != pClient)
	{
		pClient->Close();
	}
	delete pClient;
}
bool CClientPakcageMgr::Send(uint32_t uKey, std::string& buf, FileSign sign)
{
	if (m_Client.find(uKey) == m_Client.end())
	{
		////assert(false);
		return false;
	}
	CTcpClient* pClient = m_Client[uKey];
	if (FS_FILE_INFO == sign)
	{
		pClient->SetFlag(1);  //该socket 接下来接收的数据都是文件数据
	}
	if (NULL == pClient)
		return false;
	return pClient->Send(buf);
}
void CClientPakcageMgr::DispatchPkg(uint32_t uKey, CPkgDecode& pkg)
{
	uint16_t	command_id = pkg.GetCmd();
	std::string buf = pkg.GetBuf();
	switch (command_id)
	{
	case FS_SEVER_FILE_INFO_LIST:  //收到服务端发送文件信息列表
		if (NULL != m_pSink)
			m_pSink->OnFileCmd(FS_SEVER_FILE_INFO_LIST, uKey, buf);
		break;
	default:
		break;
	}
}
void CClientPakcageMgr::OnConnect(int uCode, Netfd fd)
{
	if (0 != uCode)
	{
		if (m_Client.find(fd) == m_Client.end())
		{
			//assert(false);
			return;
		}
		m_pSink->OnFileError(fd, uCode);
		CTcpClient* pClient = m_Client[fd];
		if (NULL != pClient)
		{
			pClient->Close();
			m_Client.erase(fd);
			delete pClient;
		}
	}
	else
	{
		Singleton<CFileClientMgr>::Instance().OnConnectRes(fd, uCode);
	}

}
void CClientPakcageMgr::OnFdRead(Netfd fd, const char* szBuf, int len)
{
	std::string buf;
	buf.assign(szBuf, len);
	if (m_Client.find(fd) != m_Client.end())
	{
		CTcpClient* pClient = m_Client[fd];
		if (NULL != pClient)
		{
			if (1 == pClient->GetFlag())
			{
				Singleton<CFileClientMgr>::Instance().OnFileRecive(fd, buf);    //接收的是文件数据
				return;
			}
		}
	}
	m_decode.AppendBuf(fd, buf);
	CPkgDecode pkg;
	while (m_decode.GetUsePkg(fd, pkg))
		DispatchPkg(fd, pkg);
}
void CClientPakcageMgr::OnFdWrite(Netfd fd)
{

}