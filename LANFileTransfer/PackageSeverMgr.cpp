#include "stdafx.h"
#include "PackageSeverMgr.h"
#include "DecodeBuf.h"
#include"Singleton.h"
#include "FileSeverMgr.h"
#include "Singleton.h"
#include "CodecPkg.h"

CPackageSeverMgr::CPackageSeverMgr()
{
	m_pSink = NULL;
}
void CPackageSeverMgr::RegisterSink(Util::FILE_EVENT::CFileEvent* pSink)
{
	m_pSink = pSink;
	Util::LibEventSever::Reg(this);   //注册事件
}

CPackageSeverMgr::~CPackageSeverMgr()
{
	//Util::LibEventSever::Reg(NULL);   //注册事件
}

bool CPackageSeverMgr::SendFileCmd(uint32_t uKey, FileSign cmd, std::string& buf)
{
	CPkgCode pkg;
	if (!CCodecPkg::MakePkg(pkg, cmd, buf.length(), (uint8_t*)buf.c_str()))
	{
		return false;
	}
	std::string szBuf;
	szBuf.assign(pkg.GetCodeBuf());
	return Util::LibEventSever::Send(uKey, szBuf.c_str(), szBuf.length());
}
uint32_t CPackageSeverMgr::SendFileData(uint32_t uKey, std::string& buf)
{
	Util::LibEventSever::SetFdCallBack(uKey, true, true);   //开启回调
	return Util::LibEventSever::Send(uKey, buf.c_str(), buf.length());
}

 void CPackageSeverMgr::OnAccept(Netfd fd)
 {

 }
//以下事件 回调每一个连接的fd 对应一个线程 上层应注意这一点 若有共享数据 则需加锁保护
 void CPackageSeverMgr::OnFdWrite(Netfd fd)
 {
	 if(NULL!=m_pSink)
		 m_pSink->OnFileSend(fd);   //通知上层发送文件
 }
 void CPackageSeverMgr::OnFdRead(Netfd fd, const char* szBuf, int len)
 {
	 std::string buf;
	 buf.assign(szBuf, len);
	 m_decode.AppendBuf(fd, buf);
	 CPkgDecode pkg;
	 while (m_decode.GetUsePkg(fd, pkg))
	 {
		 DispatchPkg(fd, pkg);     //获取所有可用的数据包
	 }
	 
 }
 void CPackageSeverMgr::OnFdConnet(Netfd fd, int code)
 {
	 if (NULL != m_pSink)
	 {
		 m_pSink->OnFileError(fd, 1);
	 }
 }
void CPackageSeverMgr::DispatchPkg(uint32_t uKey, CPkgDecode& pKg)
{
	uint16_t	command_id = pKg.GetCmd();
	std::string buf = pKg.GetBuf();
	FileSign cmd = (FileSign)command_id;
	switch (cmd)
	{
	case FS_INIT:
	case FS_CLIENT_AGREEDIRFILE:
	case FS_SEVER_FILE_INFO_LIST:
	case FS_FILE_INFO:
	case FS_FILE_SUCCESS:
	case FS_FILE_OVER:
	case FS_FILE_CANCLE:
		if (NULL != m_pSink)
			m_pSink->OnFileCmd((FileSign)command_id, uKey, buf);
		break;
	default:
		//assert(false);
		break;
	}
}
void CPackageSeverMgr::Close(uint32_t uKey)
{
	Util::LibEventSever::Stop(uKey);
}
