#pragma once
#include <stdint.h>
class  CBaseFile
{
public:
	CBaseFile();
	~CBaseFile();
public:
	bool OpenFile(LPCTSTR strFilePath, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition);
	uint64_t GetFileLen();
	//uFileSeekCount 移动的字节数 uFileMethod基准位置
	bool   SetFilePoint(uint64_t uFileSeekCount,uint32_t uBasePos);
	//获取 文件内容 buffer 返回实际读取的字节数  
	uint64_t GetFileContent(uint8_t* pBuffer, uint64_t uBufferSize);
	//pBuffer buffer指针，uBufferSize 写入数据的字节大小   uRealSize 实际写入的数据大小
	bool WriteFileContent(uint8_t* pBuffer, uint64_t uBufferSize,uint64_t& uRealSize);
	void CloseFile();
	//创建多级目录
	static bool CreateMultiDir(LPCTSTR lpDirPath);
private:
	HANDLE m_hHande;
};

