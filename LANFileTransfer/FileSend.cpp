#include "stdafx.h"
#include "FileSend.h"

CFileSend::CFileSend()
{
	m_bStartFlag = false;
	m_uCurrentFileSize = 0;
	m_uBuff = NULL;
	m_uOffSet = 0;
	m_bRelease = false;
	m_bNotifyProgress = false;
	m_time.Init();
}


CFileSend::~CFileSend()
{
	if (NULL != m_uBuff)
	{
		delete m_uBuff;
	}
	m_uBuff = NULL;
}
void CFileSend::SetFileExt(CFileExt* pext)
{
	m_Ext = *pext;
}
CFileExt* CFileSend::GetFileExt()
{
	return &m_Ext;
}
bool CFileSend::IsStart()
{
	return m_bStartFlag;
}
bool CFileSend::Start(bool bFlag)
{
	m_bStartFlag = bFlag;
	SignalFileObj obj;
	if (!m_Ext.GetSignalFileObj(m_szGuid, obj))   //根据当前的guiid 获取到对应文件数据
		return false;
	m_uCurrentFileSize = obj.uFileSize;
	std::wstring szPath = CvtA2W(obj.filepath);
	//m_file.CloseFile();  //关闭上一个文件句柄
	m_file.Close();
	return m_file.Open(obj.filepath, FOpen::R_B_PLUS_MODE_TYPE);
	//return m_file.OpenFile(szPath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
}
void CFileSend::SetCurrentSendFile(std::string guid)
{
	m_szGuid = guid;
}
std::string CFileSend::GetCurrentSendFile()
{
	return m_szGuid;
}
uint64_t CFileSend::GetCurrentSendFileSize()
{
	return m_uCurrentFileSize;
}
uint64_t CFileSend::GetObjectSendSize()
{
	return m_uOffSet+m_Ext.GetAlreadySendSize();     //当前文件发送的偏移量加上 已经成功发送的文件大小
}
void CFileSend::SetOffset(uint64_t uOffset)
{
	m_uOffSet = uOffset;
}
uint64_t CFileSend::GetOffSet()
{
	return m_uOffSet;
}
char* CFileSend::GetSendBuf(uint64_t& uLen)
{
	if (NULL == m_uBuff)
	{
		m_uBuff = new char[IPMSG_DEFAULT_IOBUFMAX + 1];
	}
	if (m_uOffSet == m_uCurrentFileSize)
	{
		return NULL;   //文件发送完毕
	}
	memset(m_uBuff, 0, IPMSG_DEFAULT_IOBUFMAX + 1);
	uint64_t	offset = m_uOffSet % IPMSG_DEFAULT_IOBUFMAX;
	int		readSize = IPMSG_DEFAULT_IOBUFMAX - (int)offset;
	//uLen =m_file.GetFileContent((uint8_t*)m_uBuff, readSize);
	uLen = m_file.Read(m_uBuff, 1, readSize);
	m_uOffSet += uLen;  //偏移量后移
	if (m_time.IsTimeOut())
	{
		m_bNotifyProgress = true;
		m_time.Update();
	}
	else
	{
		m_bNotifyProgress = false;
	}
	return m_uBuff;
}
bool CFileSend::IsComplete()
{
	if (m_uOffSet == m_uCurrentFileSize)
		return true;
	return false;
}
void CFileSend::SetFileFlag(std::string guid, bool bFlag)
{
	m_Ext.SetFileFlag(bFlag, guid);
}
void CFileSend::Clear()
{
	m_uOffSet = 0;
	m_uCurrentFileSize = 0;
	m_bStartFlag = false;
}
void CFileSend::SetRelease(bool bRelease)
{
	 m_bRelease= bRelease;
}
bool CFileSend::IsRelease()
{
	return m_bRelease;
}
bool CFileSend::IsNotifyProgress()
{
	return m_bNotifyProgress;
}