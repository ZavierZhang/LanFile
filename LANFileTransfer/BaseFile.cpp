#include "stdafx.h"
#include "BaseFile.h"
#include <Shlobj.h>

CBaseFile::CBaseFile()
{
	m_hHande = NULL;
}


CBaseFile::~CBaseFile()
{
	if (NULL != m_hHande)
	{
		CloseHandle(m_hHande);
		m_hHande = NULL;
	}
}
bool CBaseFile::OpenFile(LPCTSTR strFilePath, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition)
{
	HANDLE hFileHandle = CreateFile(strFilePath, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, NULL, NULL);
	DWORD D = GetLastError();
	if (INVALID_HANDLE_VALUE == hFileHandle)
	{
		CloseHandle(hFileHandle);
		return false;
	}
	m_hHande = hFileHandle;
	return true;
}
uint64_t CBaseFile::GetFileLen()
{
	if (NULL == m_hHande)
	{
		return 0;
	}
	DWORD dfileSize = GetFileSize(m_hHande, NULL);
	return dfileSize;
}
bool CBaseFile::SetFilePoint(uint64_t uFileSeekCount, uint32_t uBasePos)
{
	if (NULL == m_hHande)
		return false;
	DWORD dwRes = SetFilePointer(m_hHande, (uint64_t)uFileSeekCount, 0, uBasePos);
	if (INVALID_SET_FILE_POINTER == dwRes)
	{
		return false;
	}
	return true;
}
//获取 文件内容 buffer 返回实际读取的字节数  
uint64_t CBaseFile::GetFileContent(uint8_t* pBuffer, uint64_t uBufferSize)
{
	if (NULL == m_hHande)
		return 0;
	DWORD readsize = 0;
	ReadFile(m_hHande, pBuffer, (DWORD)uBufferSize, &readsize, NULL);
	return readsize;
}
bool CBaseFile::WriteFileContent(uint8_t* pBuffer, uint64_t uBufferSize, uint64_t& uRealSize)
{
	if (NULL == m_hHande)
	{
		return false;
	}
	DWORD dwRealSize = 0;
	bool bSuc=WriteFile(m_hHande, pBuffer, uBufferSize, &dwRealSize, NULL);
	DWORD D = GetLastError();
	uRealSize=dwRealSize;
	return bSuc;
}
bool CBaseFile::CreateMultiDir(LPCTSTR lpDirPath)
{
	int code=SHCreateDirectoryEx(NULL,lpDirPath,NULL);
	if (ERROR_ALREADY_EXISTS == code || ERROR_SUCCESS == code)
		return true;
	return false;
}

void CBaseFile::CloseFile()
{
	if (NULL != m_hHande)
	{
		CloseHandle(m_hHande);
		m_hHande = NULL;
	}
}
