#pragma once
#include "FileExt.h"
#ifdef WIN32
#include "BaseFile.h"
#endif
#include "UtilString.h"
#include "file_open.h"
#include "TimeOut.h"
#define IPMSG_DEFAULT_IOBUFMAX		(4*1024*1024)	// 4MB
//#define IPMSG_DEFAULT_IOBUFMAX		100*1024	// 4MB
//�ļ����Ͷ���
class CFileSend
{
public:
	CFileSend();
	~CFileSend();
	void SetFileExt(CFileExt* pext);
	CFileExt* GetFileExt();
	bool Start(bool bFlag);
	bool IsStart();
	void SetRelease(bool bRelease);
	bool IsRelease();
	void SetOffset(uint64_t uOffset);
	uint64_t GetOffSet();
	//���õ�ǰ�����ļ���guiid
	void SetCurrentSendFile(std::string guid);
	//��ȡ��ǰ��Ҫ���͵��ļ�
	std::string GetCurrentSendFile();
	//��ȡ��ǰ���ڷ��͵��ļ��Ĵ�С
	uint64_t GetCurrentSendFileSize();
	//��ȡ�ôζ����ܹ����͵��ļ���С����С����Ŀ¼�ѷ����ļ��Ĵ�С��
	uint64_t GetObjectSendSize();
	//��ȡ�������ݵ�buf ����buf  ulenΪ����buf�ĳ���
	char* GetSendBuf(uint64_t& uLen);
	bool IsComplete();  //��ǰ�ļ��Ƿ������
	void SetFileFlag(std::string guid, bool bFlag);
	void Clear();
	bool IsNotifyProgress();
private:
	bool m_bRelease;
	bool m_bStartFlag;  //�Ƿ��Ѿ���ʼ������
	uint64_t m_uOffSet;  //��ǰ�����ļ���ƫ����
	uint64_t m_uCurrentFileSize;  //��ǰ���ڷ��͵��ļ��Ĵ�С
	CFileExt m_Ext;
	//CBaseFile m_file; //�����ļ�����
	FOpen m_file;
	std::string m_szFilePath;  //��ǰ���ڷ��͵��ļ�·��
	std::string m_szGuid;  //��ǰ�����ļ���Ӧ��GUIID
	char* m_uBuff;  //�ļ�����buff��ָ��
	bool m_bNotifyProgress;
	CTimeOut  m_time;
};

