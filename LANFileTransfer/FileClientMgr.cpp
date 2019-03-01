#include "stdafx.h"
#include "FileClientMgr.h"
#include "Singleton.h"
#include "ClientPakcageMgr.h"
#include "ClientManagement.h"
#include "CheckNetMgr.h"
#include "CodecPkg.h"
#include "FileDataJson.h"
#include "UtilTool.h"
CFileClientMgr::CFileClientMgr()
{
	Singleton<CClientPakcageMgr>::Instance().RegisterSink(this);
}


CFileClientMgr::~CFileClientMgr()
{
	Singleton<CClientPakcageMgr>::Instance().RegisterSink(NULL);
	Singleton<CClientPakcageMgr>::UnInstance();
}
void CFileClientMgr::OnFileCmd(FileSign cmd, uint32_t uKey, std::string& buf)
{
	switch (cmd)
	{
	case FS_SEVER_FILE_INFO_LIST:
		OnReciveSeverFileList(uKey, buf);
		break;
	default:
		break;
	}
}
void CFileClientMgr::OnFileRecive(uint32_t uKey, std::string& buf)
{
	CFileRecv* pRecive = m_FileQueue.GetWorkFileObj(uKey);
	if (NULL == pRecive)
	{
		LOG_WIN_E("not get the object ukey=" << uKey);
		return;
	}
	std::string sessionid = pRecive->GetSessionID();

	if (!pRecive->ReciveFile(buf))
	{
		LOG_WIN_E("recive the file buf fail ukey=" << uKey<<"sessionid ="<<sessionid);
		NotifyResult(sessionid.c_str(), 1005);
		m_FileQueue.StopFile(uKey);
		m_FileQueue.DeleteWorkFile(uKey);
		return;
	}
	uint64_t uAlreadySize = pRecive->GetAlreadyRecvSize();

	if(pRecive->IsNotifyProgress())
		NotifyProgress(sessionid.c_str(), uAlreadySize, pRecive->GetFileExt().GetFileAllSize());

	if (pRecive->CheckComplete() && pRecive->CheckMD5())  //检测当前文件大小接收完毕
	{
		std::string guid = pRecive->GetCurrentReciveGuid();
		std::string buf;
		CFileDataJson::PackageReciveOver(uKey, buf, guid, sessionid);
		pRecive->SetFileOkFlag(true, guid);
		pRecive->Clear();
		SendFileCmd(uKey, FS_FILE_SUCCESS, buf);   //当前文件接收成功
		if (pRecive->CheckSessionOK())
		{
			CFileDataJson::PackageSessionOver(uKey, buf, sessionid);
			SendFileCmd(uKey, FS_FILE_OVER, buf);
			NotifyResult(sessionid, 0);
			m_FileQueue.StopFile(uKey); 
		}
		else
		{
			//继续获取下一个文件
			std::string guidnext = pRecive->GetNextReciveGuid();
			pRecive->SetCurrentReciveFile(guidnext);
			std::string buf1;
			CFileDataJson::PackageReqFileInfo(uKey, guidnext, pRecive->GetFileExt(), buf1);
			SendFileCmd(uKey, FS_FILE_INFO, buf1);
			pRecive->Start(true);
		}
	}

	if (pRecive->IsRelease())
	{
		m_FileQueue.DeleteWorkFile(uKey);
		return;
	}
}
bool CFileClientMgr::SendFileCmd(uint32_t uKey, FileSign cmd, std::string& buf)
{
	bool bOK = false;
	CPkgCode pkg;
	if(!CCodecPkg::MakePkg(pkg, cmd,buf.length(),(uint8_t*)buf.c_str()))
	{
		LOG_WIN_E("make pkg error" << uKey);
		return false;
	}
	std::string szBuf;
	szBuf.assign(pkg.GetCodeBuf());
	if (!Singleton<CClientPakcageMgr>::Instance().Send(uKey, szBuf,cmd))  //发送请求文件的数据
	{
		LOG_WIN_E("send buf error" << uKey);
		return false;
	}
	return true;
}
//收到文件列表数据了
void CFileClientMgr::OnReciveSeverFileList(uint32_t uKey, std::string& buf)
{
	CFileExt ext;
	if (!CFileDataJson::ParseFileListPkg(uKey, buf, ext))   //解析数据
	{
		LOG_WIN_E("ParseFileListPkg is fail " <<"ukey="<<uKey<<"buf="<<buf);
		NotifyResult(ext.GetSessionID(), 1001);  //通知外面失败
		return;
	}
	std::string guid ,sessionid;
	sessionid = ext.GetSessionID();
	if (!m_FileQueue.AddWorkFile(uKey, ext, guid))
	{
		LOG_WIN_E("AddWorkFile is fail " << "ukey=" << uKey << "guid=" << guid);
		NotifyResult(sessionid, 1003);  //通知外面失败
		return;
	}
	std::string buf1;
	if (!CFileDataJson::PackageReqFileInfo(uKey, guid, ext, buf1))     //组装请求的文件信息
	{
		LOG_WIN_E("PackageReqFileInfo is fail " << "ukey=" << uKey);
		m_FileQueue.DeleteWorkFile(uKey);
		NotifyResult(sessionid, 1003);  //通知外面失败
		return;
	}
	SendFileCmd(uKey, FS_FILE_INFO, buf1);  //发送请求文件的信息  等待服务端回应即可开始接受数据信息
}
//上层点击了接受文件
bool CFileClientMgr::ReciveFile(const char*szSessionID, const char* szFilePath, const char* ip, int nPort)
{
	uint32_t uKey = 0;
	Singleton<CClientPakcageMgr>::Instance().Connect(ip, nPort, uKey);
	CFileWaitConnect obj;
	obj.ip = ip;
	obj.port = nPort;
	obj.szSessionID = szSessionID;
	obj.uKey = uKey;
	obj.szRecvPath = szFilePath;
	m_FileQueue.AddFile(uKey, obj);
	return true;
}
bool CFileClientMgr::CancelRecive(const char*szSessionID)
{
	std::string sessionid = szSessionID;
	uint32_t uKey=m_FileQueue.GetConnKey(sessionid);
	if (0 == uKey)
	{
		LOG_WIN_E("get the connect key fail="<< ",sessionid=" << sessionid);
		return false;
	}
	Singleton<CClientPakcageMgr>::Instance().DisConnect(uKey);
	m_FileQueue.StopFile(uKey);
	return true;
}
void CFileClientMgr::StopAll()
{
	std::map<std::string, uint32_t> mapTemp = m_FileQueue.GetAllLink();
	for (std::map<std::string, uint32_t>::iterator iter = mapTemp.begin(); iter != mapTemp.end(); iter++)
	{
		uint32_t uKey = iter->second;
		m_FileQueue.StopFile(uKey);
		Singleton<CClientPakcageMgr>::Instance().DisConnect(uKey);
	}
}
bool CFileClientMgr::SendAgreeSaveFile(uint32_t uKey,std::string sessionid)
{
	std::string sJson;
	CFileDataJson::PackageAgreeFile(uKey, sessionid, sJson);
	return SendFileCmd(uKey, FS_CLIENT_AGREEDIRFILE, sJson);  //发送客户端同意接受文件的请求
}
void CFileClientMgr::OnConnectRes(uint32_t uKey, uint32_t uCode)
{
	std::string sessionid = m_FileQueue.GetConnSessionID(uKey);
	if (0 != uCode)
	{
		//链接失败
		LOG_WIN_E("connect is fail=" << uKey << ",sessionid=" << sessionid);
		NotifyResult(sessionid.c_str(), 1000);
		return ;
	}
	LOG_WIN_I("connect is ok uKey="<<uKey<<",sessionid="<<sessionid);
	SendAgreeSaveFile(uKey,sessionid);   //发送同意接受文件请求
}
void CFileClientMgr::OnFileError(uint32_t uKey, uint32_t uCode)
{
	CFileRecv* pRecive =m_FileQueue.GetWorkFileObj(uKey);
	if (NULL == pRecive)
	{
		LOG_WIN_E("on file error " << uKey << "  uCode=" << uCode);
		return;
	}
	std::string sessionid = pRecive->GetSessionID();
	LOG_WIN_E("on file error " << uKey << ",sessionid=" << sessionid<<"  uCode="<<uCode);
	m_FileQueue.DeleteWorkFile(uKey);
	//NotifyResult(sessionid, uCode);
}

void CFileClientMgr::NotifyResult(std::string sessionid, uint32_t uCode)
{
	Singleton<ClientManagement>::Instance().OnResult(sessionid.c_str(), uCode);
}
void CFileClientMgr::NotifyProgress(const char* szSesssionID, uint64_t uPos, uint64_t uTotal)
{
	Singleton<ClientManagement>::Instance().OnPorgress(szSesssionID, uPos, uTotal);
}