#include "stdafx.h"
#include "FileExt.h"
#include "UtilString.h"
#include "FileMd5.h"
#include "UtilTool.h"
#include <sys/stat.h>
#ifdef WIN32
#else
#include <fcntl.h>             // �ṩopen()����
#include <unistd.h>
#include <sys/stat.h>        // �ṩ���Բ�������
#include <sys/types.h>         // �ṩmode_t ����
#include <dirent.h>  
#include <limits.h>
#endif // OS_MAC_OS


//��ȡ�ļ����� �����ļ��� ���ȡ�ļ�������
std::string GetFileDirName(std::string path)
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
	int len = path.length() - nIndex;
	std::string name = path.substr(nIndex+1, len);
	return name;
}
//��ȡ ��Ŀ¼





CFileExt::CFileExt()
{
	m_bIsDirectory = false;
	m_uAllFileSize = 0;
	m_bParsing = false;
}


CFileExt::~CFileExt()
{
}

void CFileExt::SetPath(const char* szPath)
{
	m_szPath = szPath;
#ifdef WIN32
	std::wstring szTemp = CvtA2W(m_szPath);
	DWORD dwAttr = ::GetFileAttributes(szTemp.c_str());
	m_bIsDirectory = (((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) ? true : false);
#else 
	struct stat buf;
	if (0 == lstat(szPath, &buf))
	{
		if (S_ISREG(buf.st_mode))
			m_bIsDirectory = false;
		else if(S_ISDIR(buf.st_mode))
			m_bIsDirectory = true;
	}
#endif
	m_szFileName = GetFileDirName(m_szPath);
}
void CFileExt::SetDirName(const char* szFileName)
{
	m_szFileName = szFileName;
}
std::string CFileExt::GetDirName()
{
	return m_szFileName;
}
std::string CFileExt::GetPath()
{
	return m_szPath;
}
bool CFileExt::IsDirectory()
{
	return m_bIsDirectory;
}
void CFileExt::SetDirectory(bool bDir)
{
	m_bIsDirectory = bDir;
}
uint64_t CFileExt::GetFileLength(std::wstring szFileName)
{
#ifdef WIN32
	WIN32_FIND_DATA		rFileInfo = { 0 };
	LARGE_INTEGER		rFileSize = { 0 };
	HANDLE hFind = FindFirstFile(szFileName.c_str(), &rFileInfo);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		rFileSize.LowPart = rFileInfo.nFileSizeLow;
		rFileSize.HighPart = rFileInfo.nFileSizeHigh;
		FindClose(hFind);
	}
	return rFileSize.QuadPart;
#else 
	struct stat buf;
	if (0 == lstat(CvtW2A(szFileName).c_str(), &buf))
	{
		return buf.st_size;
	}
	return 0;
#endif
}
std::string CFileExt::GetFileName()
{
	return m_szFileName;
}
std::map<std::string,SignalFileObj>& CFileExt::GetFileList()
{
	return m_szFileList;
}
 bool CFileExt::GetSignalFileObj(std::string szGuiid, SignalFileObj &obj)
{
	if (m_szFileList.find(szGuiid) == m_szFileList.end())
	{
		return false;
	}
	obj = m_szFileList[szGuiid];
	return true;
}
std::string CFileExt::GetGUID()
{
	return m_szGUID;
}
std::string CFileExt::GetSessionID()
{
	return m_szSessionID;
}
void CFileExt::SetSessionID(std::string sessionID)
{
	m_szSessionID = sessionID;
}
void CFileExt::SetFileAllSize(uint64_t uFileSize)
{
	m_uAllFileSize = uFileSize;
}
uint64_t CFileExt::GetFileAllSize()
{
	return m_uAllFileSize;
}
uint64_t CFileExt::GetAlreadySendSize()
{
	uint64_t uFileSize = 0;
	for (std::map<std::string, SignalFileObj>::iterator iter = m_szFileList.begin(); iter != m_szFileList.end(); iter++)
	{
		if (iter->second.bOK == true)
		{
			uFileSize += iter->second.uFileSize;
		}
	}
	return uFileSize;
}
void CFileExt::SetFileFlag(bool bFlag, std::string guid)
{
	std::map<std::string, SignalFileObj>::iterator iter = m_szFileList.find(guid);
	if (iter != m_szFileList.end())
	{
		iter->second.bOK = true;
	}

}

//uinix �ݹ��ȡ����·���ļ�
void CFileExt::scan_dir(const char *dir, const char* srcdir)   // ����Ŀ¼ɨ�躯��
{
#ifdef WIN32
#else
	DIR *dp;                      // ������Ŀ¼��ָ��
	struct dirent *entry;         // ����dirent�ṹָ�뱣�����Ŀ¼
	struct stat statbuf;          // ����statbuf�ṹ�����ļ�����
	if ((dp = opendir(dir)) == NULL) // ��Ŀ¼����ȡ��Ŀ¼��ָ�룬�жϲ����Ƿ�ɹ�
	{
		//puts("can't open dir.");
		return;
	}
	chdir(dir);                     // �л�����ǰĿ¼
	while ((entry = readdir(dp)) != NULL)  // ��ȡ��һ��Ŀ¼��Ϣ�����δ����ѭ��
	{
		lstat(entry->d_name, &statbuf); // ��ȡ��һ����Ա����
		if (S_IFDIR &statbuf.st_mode)    // �ж���һ����Ա�Ƿ���Ŀ¼
		{
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
				continue;

			//printf("%*s%s/\n", depth, "", entry->d_name);  // ���Ŀ¼����
			scan_dir(entry->d_name, srcdir);              // �ݹ��������ɨ����һ��Ŀ¼������
		}
		else
		{
			//printf("%*s%s\n", depth, "", entry->d_name);  // ������Բ���Ŀ¼�ĳ�Ա
			char szPath[PATH_MAX] = { 0 };
			realpath(entry->d_name, szPath);
			std::string szFile = szPath;
			std::string strSourceFolder = srcdir;
			std::string szFileName=szFile.substr(strSourceFolder.length() + 1, szFile.length() - strSourceFolder.length());
			std::wstring szWFile = CvtA2W(szFile);
			//std::cout << path1 << "\n";
			SignalFileObj obj;
			obj.uFileSize = GetFileLength(szWFile);
			m_uAllFileSize += obj.uFileSize;
			obj.filename = szFileName;
			obj.filepath = szFile;
			//CFileMd5 md5;
			CMD5 md5;
			std::string strmd5;
			md5.GetMd5(szWFile, strmd5);
			obj.filemd5 = strmd5;
			std::string szGuid = Util::Tool::GetGuid();
			m_szFileList[szGuid] = obj;

		}

	}
	chdir("..");                                                  // �ص��ϼ�Ŀ¼
	closedir(dp);                                                 // �ر���Ŀ¼��
#endif

}


bool CFileExt::FindDirectoryAllFiles(std::wstring szPath, std::wstring strSourceFolder)
{
#ifdef WIN32
	if (szPath.empty())
		return false;
	std::wstring wpath= szPath;
	WIN32_FIND_DATA FindFileData;

	wpath += L"\\*.*";
	HANDLE hFind = ::FindFirstFile(wpath.c_str(), &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind) 
		return false;

	while (FindNextFile(hFind, &FindFileData))
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != L'.')
			{
				std::wstring szFile = szPath + std::wstring(L"\\") + std::wstring(FindFileData.cFileName);
				FindDirectoryAllFiles(szFile.c_str(), strSourceFolder);
			}
		}
		else
		{
			std::wstring szFile = szPath + std::wstring(L"\\") + std::wstring(FindFileData.cFileName);
			std::wstring filename =szFile.substr(strSourceFolder.length()+1,szFile.length() - strSourceFolder.length());
			SignalFileObj obj;
			obj.uFileSize = GetFileLength(szFile);
			m_uAllFileSize += obj.uFileSize;
			std::string  szFileName = CvtW2A(filename);
			std::string szFileFullPath= CvtW2A(szFile);
			obj.filename = szFileName;
			obj.filepath = szFileFullPath;
			CMD5 md5;
			std::string strmd5;
			md5.GetMd5(szFile, strmd5);
			obj.filemd5 = strmd5;
			std::string szGuid = Util::Tool::GetGuid();
			m_szFileList[szGuid]=obj;
		}
	}
	FindClose(hFind);
	return true;
#else
	return false;
#endif
}
bool CFileExt::Parsing()
{
	if (m_bParsing)
	{
		return true;
	}
	std::wstring filepath = CvtA2W(m_szPath);
	if (!m_bIsDirectory)
	{
		CMD5 md5;
		std::string strmd5;
		md5.GetMd5(filepath, strmd5);
		m_uAllFileSize = GetFileLength(filepath);
		m_szGUID= Util::Tool::GetGuid();
		SignalFileObj obj;
		obj.filepath = m_szPath;
		obj.guid = m_szGUID;
		obj.uFileSize = m_uAllFileSize;
		obj.filename = m_szFileName;
		obj.filemd5 = strmd5;
		m_szFileList[m_szGUID] = obj;
	}
	else
	{
#ifdef WIN32
		FindDirectoryAllFiles(filepath, filepath);
#else 
		scan_dir(m_szPath.c_str(),m_szPath.c_str());
#endif
	}
	m_bParsing = true;
	return true;
}
