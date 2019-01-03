#pragma once
#include "FileExt.h"
#include "RapidJson.h"
class CFileSendJsonData
{
public:
	CFileSendJsonData();
	~CFileSendJsonData();
	//�����ͻ���ͬ������ļ�������
	static bool ParseFileAgreeMsg(std::string& inBuf, std::string& sessionid);
	//��װ�ļ��б���Ϣ
	static bool PackageFileList(CFileExt& ext, std::string& sessionid,std::string& outBuf);
	//�����ļ�
	static bool ParseReqSendFile(std::string& inBuf, std::string& sessionid, std::string& guid, std::string& md5);
	//����һ���ļ��ļ��������buf
	static bool ParseCompleteSendFile(std::string& inBuf, std::string& sessionid, std::string& guid);
};

