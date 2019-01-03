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
	//uFileSeekCount �ƶ����ֽ��� uFileMethod��׼λ��
	bool   SetFilePoint(uint64_t uFileSeekCount,uint32_t uBasePos);
	//��ȡ �ļ����� buffer ����ʵ�ʶ�ȡ���ֽ���  
	uint64_t GetFileContent(uint8_t* pBuffer, uint64_t uBufferSize);
	//pBuffer bufferָ�룬uBufferSize д�����ݵ��ֽڴ�С   uRealSize ʵ��д������ݴ�С
	bool WriteFileContent(uint8_t* pBuffer, uint64_t uBufferSize,uint64_t& uRealSize);
	void CloseFile();
	//�����༶Ŀ¼
	static bool CreateMultiDir(LPCTSTR lpDirPath);
private:
	HANDLE m_hHande;
};

