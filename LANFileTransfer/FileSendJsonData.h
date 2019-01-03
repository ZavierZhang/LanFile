#pragma once
#include "FileExt.h"
#include "RapidJson.h"
class CFileSendJsonData
{
public:
	CFileSendJsonData();
	~CFileSendJsonData();
	//解析客户端同意接受文件的命令
	static bool ParseFileAgreeMsg(std::string& inBuf, std::string& sessionid);
	//组装文件列表信息
	static bool PackageFileList(CFileExt& ext, std::string& sessionid,std::string& outBuf);
	//解析文件
	static bool ParseReqSendFile(std::string& inBuf, std::string& sessionid, std::string& guid, std::string& md5);
	//解析一个文件文件完成命令buf
	static bool ParseCompleteSendFile(std::string& inBuf, std::string& sessionid, std::string& guid);
};

