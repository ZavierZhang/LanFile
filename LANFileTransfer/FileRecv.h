#pragma once
#include"FileExt.h"
#include "TimeOut.h"
#include "file_open.h"
#ifdef WIN32
#include "BaseFile.h"
#endif // WIN32
//�ļ����ն���
class CFileRecv
{
public:
	CFileRecv();
	~CFileRecv();
	void SetFileExt(CFileExt* pext);
	bool Start(bool bFlag);
	void SetRelease(bool bRelease);
	bool IsRelease();
	void SetRecvBasePath(std::string path);
	bool IsStart();
	//���õ�ǰ�����ļ���guiid
	void SetCurrentReciveFile(std::string guid);
	//��ȡ��ǰ��Ҫ���͵��ļ�
	std::string GetCurrentReciveFile();
	//��ȡ��һ��Ҫ�����ļ���guid ���Ϊ�����ʾ���
	std::string GetNextReciveGuid();
	std::string GetCurrentReciveGuid();
	std::string GetSessionID();
	CFileExt& GetFileExt();
	bool ReciveFile(std::string& databuf);
	bool CheckComplete(); //��ǰ���յ��ļ��Ƿ����
	bool CheckMD5();  //���MD5
	bool CheckSessionOK();   //��⵱ǰ�ļ������Ƿ����
	void SetFileOkFlag(bool bFlag,std::string guid);
	void Clear();
	bool IsNotifyProgress();
	uint64_t GetAlreadyRecvSize();
private:
	bool m_bRelease;
	bool m_bStartFlag;  //�Ƿ��Ѿ���ʼ������
	uint64_t m_uOffSet;  //�ļ�ƫ����  ��ǰ�ѽ����ļ���С
	uint64_t m_uFileSize; //��ǰ�����ļ��Ĵ�С
	CFileExt m_Ext;
	//CBaseFile m_file; //�����ļ�����
	FOpen m_file;
	std::string m_szBaseFilePath;  //��ǰ���յĻ�׼·��  
	std::string m_szFilePath;   //��ǰ���ڽ����ļ���·��
	std::string m_szGuid;  //��ǰ���ڽ����ļ���guiid
	char* m_uBuff;  //�ļ�����buff��ָ��
	bool m_bNotifyProgress;
	CTimeOut  m_time;
};

