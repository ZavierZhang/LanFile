#pragma once
#include "FileExt.h"
//json ������װ����������
class CFileDataJson
{
public:
	CFileDataJson();
	~CFileDataJson();
	//�ͻ���ͬ������ļ�
	static bool PackageAgreeFile(uint32_t uKey, std::string sessionid,std::string& outbuf);

	//��ѹ�ļ���Ϣ�б��buf
	static bool ParseFileListPkg(uint32_t uKey, std::string& buf, CFileExt& ext);
	//�������ĳһ���ļ�����Ϣ�ļ���guid guid
	static bool PackageReqFileInfo(uint32_t uKey, std::string guid, CFileExt& ext, std::string& outbuf);
	//ĳһ���ļ� �������
	static bool PackageReciveOver(uint32_t uKey, std::string& buf, std::string guid, std::string sessionid);
	//���λỰ��������
	static bool PackageSessionOver(uint32_t uKey, std::string& buf, std::string sessionid);


};

