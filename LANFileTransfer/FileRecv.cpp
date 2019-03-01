#include "stdafx.h"
#include "FileRecv.h"
#include "UtilString.h"

CFileRecv::CFileRecv()
{
	m_bRelease = false;
	m_bNotifyProgress = false;
	if (!CheckComplete())
	{
		remove(m_szFilePath.c_str());
	}
	Clear();
	m_time.Init();
}

std::string GetFileParentDir(std::string path)
{
	if (path.empty())
		return "";
#ifdef WIN32
	int nIndex = path.rfind("\\");
#else
	int nIndex = path.rfind("/");
#endif
	if (-1 == nIndex)
	{
		return "";
	}
	std::string name = path.substr(0, nIndex);
	return name;
}
CFileRecv::~CFileRecv()
{
}
void CFileRecv::SetFileExt(CFileExt* pext)
{
	m_Ext = *pext;
}
bool CFileRecv::IsStart()
{
	return m_bStartFlag;
}
void CFileRecv::SetRecvBasePath(std::string path)
{
	m_szBaseFilePath = path;
}
bool CFileRecv::Start(bool bFlag)
{
	m_bStartFlag = bFlag;
	SignalFileObj obj;
	if (!m_Ext.GetSignalFileObj(m_szGuid, obj))   //根据当前的guiid 获取到对应文件数据
	{
		return false;
	}
		
	std::string path = m_szBaseFilePath +obj.filename;
	if (m_Ext.IsDirectory())
	{
#ifdef WIN32
		path = m_szBaseFilePath + "\\" + m_Ext.GetDirName() + "\\" + obj.filename;
#else
		path = m_szBaseFilePath + "/" + m_Ext.GetDirName() + "/" + obj.filename;
#endif
		std::string temdir = GetFileParentDir(path);
		std::wstring wtemp = CvtA2W(temdir);

#ifdef WIN32
		if (!CBaseFile::CreateMultiDir(wtemp.c_str()))
		{
			return false;
		}
#else  
		if (!m_file.create_file_directory(temdir.c_str()))
		{
			return false;
		}
#endif		
	}
	else
	{
#ifdef WIN32
		path = m_szBaseFilePath + "\\" + obj.filename;

#else
		path = m_szBaseFilePath + "/" + obj.filename;
#endif
	}
	m_uFileSize = obj.uFileSize;  
	std::wstring szPath = CvtA2W(path);
	//m_file.CloseFile();  //关闭上一个文件句柄
	m_file.Close();
	//return m_file.OpenFile(szPath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, OPEN_ALWAYS);
	m_szFilePath = path;
	return m_file.Open(path,FOpen::A_B_PLUS_MODE_TYPE);
}
void CFileRecv::SetCurrentReciveFile(std::string guid)
{
	m_szGuid = guid;
}
std::string CFileRecv::GetCurrentReciveFile()
{
	return m_szGuid;
}
std::string CFileRecv::GetNextReciveGuid()
{
	std::map<std::string,SignalFileObj>& mapTemp=m_Ext.GetFileList();
	for (std::map<std::string, SignalFileObj>::iterator iter = mapTemp.begin(); iter != mapTemp.end(); iter++)
	{
		if (!iter->second.bOK)
			return iter->first;
	}
	return "";
}

bool CFileRecv::ReciveFile(std::string& databuf)
{
	uint64_t uRealSize = 0;
	//if (true == m_file.WriteFileContent((uint8_t*)databuf.c_str(), databuf.length(), uRealSize))
	uRealSize=m_file.Write((uint8_t*)databuf.c_str(), 1, databuf.length());
	if(0!=uRealSize)
	{
		m_uOffSet += uRealSize;
		if (m_time.IsTimeOut())
		{
			m_bNotifyProgress = true;
			m_time.Update();
		}
		else
		{
			m_bNotifyProgress = false;
		}
		return true;
	}
	return false;
}

bool CFileRecv::IsNotifyProgress()
{
	return m_bNotifyProgress;
}

bool CFileRecv::CheckComplete()
{
	if (m_uOffSet == m_uFileSize)
		return true;
	return false;
}
bool CFileRecv::CheckMD5()
{
	return true;
}
std::string CFileRecv::GetCurrentReciveGuid()
{
	return m_szGuid;
}
std::string CFileRecv::GetSessionID()
{
	return m_Ext.GetSessionID();
}
CFileExt& CFileRecv::GetFileExt()
{
	return m_Ext;
}
bool CFileRecv::CheckSessionOK()
{
	std::string ok = GetNextReciveGuid();
	if (ok.empty())
		return true;
	return false;
}
void CFileRecv::SetFileOkFlag(bool bFlag, std::string guid)
{
	m_Ext.SetFileFlag(bFlag, guid);
}
void CFileRecv::Clear()
{
	m_uFileSize = 0;
	m_uBuff = NULL;
	m_uOffSet = 0;
	//m_file.CloseFile();
	m_file.Close();
}
uint64_t CFileRecv::GetAlreadyRecvSize()
{
	return m_Ext.GetAlreadySendSize() + m_uOffSet;
}

void CFileRecv::SetRelease(bool bRelease)
{
	m_bRelease = bRelease;
}
bool CFileRecv::IsRelease()
{
	return m_bRelease;
}