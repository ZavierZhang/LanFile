#include "stdafx.h"
#include "FileSeverMgr.h"
#include "PackageSeverMgr.h"
#include"Singleton.h"
#include "FileMd5.h"
#include "FileSendJsonData.h"
#include "RapidJson.h"
#include "UtilTool.h"
CFileSeverMgr::CFileSeverMgr()
{
	Singleton<CPackageSeverMgr>::Instance().RegisterSink(this);
	m_pSink = NULL;
}

CFileSeverMgr::~CFileSeverMgr()
{
	Singleton<CPackageSeverMgr>::Instance().RegisterSink(NULL);
	Singleton<CPackageSeverMgr>::UnInstance();
}
void CFileSeverMgr::OnFileCmd(FileSign cmd, uint32_t uKey, std::string& buf)
{
	switch (cmd)
	{
	case FS_CLIENT_AGREEDIRFILE:   //客户端同意接受文件了
		OnAgreeFile(uKey, buf);
		break;
	case FS_FILE_INFO:
		OnReqSendFile(uKey, buf);
		break;
	case FS_FILE_SUCCESS:
		OnFileSendSuccess(uKey, buf);
		break;
	case FS_FILE_OVER:
		OnCompleteSendFile(uKey, buf);
		break;
	case FS_FILE_CANCLE:
		break;
	default:
		break;
	}
}
void CFileSeverMgr::OnFileRecive(uint32_t uKey, std::string& buf)
{
	;//服务端没有文件接收
}
void CFileSeverMgr::OnFileSend(uint32_t uKey)
{
	DoSendFile(uKey);
}
void CFileSeverMgr::OnFileError(uint32_t uKey, uint32_t uCode)
{
	std::string sessionid=m_Queue.GetWorkSessinID(uKey);
	m_Queue.DeleteWorkFile(uKey);
	NotifyResult(sessionid, uCode);
}
//这边以底层数据驱动来处理
bool CFileSeverMgr::DoSendFile(uint32_t uKey)
{
	CFileSend* pSend = m_Queue.GetWorkObj(uKey);
	if (NULL == pSend)
		return false;
	if(!pSend->IsComplete() )   //档次传输是否结束   这边由底层来驱动传输数据 底层发送完数据之后 再回调此接口
	{
		std::string buf;
		if (pSend->IsStart())
		{
			uint64_t uLen = 0;
			char* szBuf = pSend->GetSendBuf(uLen);
			if (NULL != szBuf && 0 != uLen)
			{
				buf.assign(szBuf, uLen);   //附加到底层buf
			}
		}
		uint32_t uCode=Singleton<CPackageSeverMgr>::Instance().SendFileData(uKey, buf);
		if (0 == uCode)   //网络错误了  发送失败   会有网络事件投递到上层 这边中断传输即可
			return false;

		if (pSend->IsNotifyProgress())
		{
			std::string sessionid = pSend->GetFileExt()->GetSessionID();
			uint64_t uAreadySendSize = pSend->GetObjectSendSize();
			uint64_t uTotalSize = pSend->GetFileExt()->GetFileAllSize();
			NotifypProgress(sessionid, uAreadySendSize, uTotalSize);
		}
	}
	if (pSend->IsComplete())
	{
		pSend->Clear();    //等待客户端请求下一个文件   当时服务端并不标志该文件已经传输结束  等待客户端发送接收成功标记
	}
	if (pSend->IsRelease())
	{
		m_Queue.DeleteWorkFile(uKey);   //删除对象
		return true;
	}
	return true;
}
bool CFileSeverMgr::SeverSendFileList(uint32_t uKey,CFileExt& ext, std::string& sessionid)
{
	std::string sJson;
	if (!CFileSendJsonData::PackageFileList(ext, sessionid, sJson))
		return false;
	return Singleton<CPackageSeverMgr>::Instance().SendFileCmd(uKey, FS_SEVER_FILE_INFO_LIST, sJson);
}
void CFileSeverMgr::OnAgreeFile(uint32_t uKey, std::string& buf)
{
	std::string sessionid, guiid,md5;
	LOG_WIN_I("CFileSeverMgr::OnAgreeFile" << uKey);
	if (!CFileSendJsonData::ParseFileAgreeMsg(buf, sessionid))
	{
		LOG_WIN_E("ParseFileAgreeMsg fail" << uKey<<"sessionid"<<sessionid);
		return;
	}
		
	CFileExt* pExt = m_Queue.GetFileExtObj(sessionid);
	if (NULL == pExt)
	{
		LOG_WIN_E("get the pExt object fail" << uKey << "sessionid" << sessionid);
		return;
	}
	pExt->Parsing();  //解析文件信息
	pExt->SetSessionID(sessionid);
	SeverSendFileList(uKey, *pExt, sessionid);//发送文件信息
}
void CFileSeverMgr::OnReqSendFile(uint32_t uKey, std::string& buf)
{
	LOG_WIN_I("CFileSeverMgr::OnReqSendFile" << uKey<<"buf ="<<buf);
	std::string sessionid, guid,md5;
	if (!CFileSendJsonData::ParseReqSendFile(buf, sessionid, guid, md5))
	{
		NotifyResult(sessionid, 1003);
		return ;
	}
	if (!m_Queue.AddWorkFile(uKey, sessionid, guid))
	{
		NotifyResult(sessionid, 1004);
		return;
	}
	DoSendFile(uKey);
}
void CFileSeverMgr::OnCompleteSendFile(uint32_t uKey, std::string& buf)
{
	std::string sessionid=m_Queue.GetWorkSessinID(uKey);
	if (!sessionid.empty())
	{
		LOG_WIN_I("complete file" << uKey << "buf ="<<buf<<"sessionid"<<sessionid);
		NotifyResult(sessionid,0);  //通知外部传输文件成功
	}
	Singleton<CPackageSeverMgr>::Instance().Close(uKey);
	m_Queue.DeleteWorkFile(uKey);
}
void CFileSeverMgr::OnFileSendSuccess(uint32_t uKey, std::string& buf)
{
	//改链接关闭write回调
	Util::LibEventSever::SetFdCallBack(uKey, true, false);
	CFileSend* pSend = m_Queue.GetWorkObj(uKey);
	if (NULL == pSend)
		return ;
	std::string sessionid, guid;
	if (CFileSendJsonData::ParseCompleteSendFile(buf, sessionid, guid))
	{
		LOG_WIN_I("CFileSeverMgr::OnFileSendSuccess  single file success" << uKey << "buf =" << buf<<"next file guid="<<guid);
		pSend->SetFileFlag(guid, true);
	}
}

void CFileSeverMgr::AddFile(const char*szSessionID, const char* szPath)
{
	std::string ID = szSessionID;
	CFileExt ext;
	ext.SetPath(szPath);
	m_Queue.AddFile(szSessionID, ext);
}
void CFileSeverMgr::StopAll()
{

}
bool CFileSeverMgr::CancelSend(const char*szSessionID)
{
	std::string ID=szSessionID;
	uint32_t uKey=m_Queue.GetConnKey(ID);
	m_Queue.StopFile(uKey);
	Singleton<CPackageSeverMgr>::Instance().Close(uKey);
	return true;
}
void CFileSeverMgr::Init(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink)
{
	m_pSink = pSink;
}

void CFileSeverMgr::NotifypProgress(std::string& sessionid, uint64_t uPos, uint64_t uTotal)
{
	if (NULL != m_pSink)
		m_pSink->OnPorgress(sessionid.c_str(),uPos ,uTotal);
}
void CFileSeverMgr::NotifyResult(std::string& sessionid, uint32_t uCode)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnResult(sessionid.c_str(), uCode);
	}
}