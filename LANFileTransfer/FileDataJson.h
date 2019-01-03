#pragma once
#include "FileExt.h"
//json 数据组装解析管理类
class CFileDataJson
{
public:
	CFileDataJson();
	~CFileDataJson();
	//客户端同意接受文件
	static bool PackageAgreeFile(uint32_t uKey, std::string sessionid,std::string& outbuf);

	//解压文件信息列表的buf
	static bool ParseFileListPkg(uint32_t uKey, std::string& buf, CFileExt& ext);
	//请求具体某一个文件的信息文件的guid guid
	static bool PackageReqFileInfo(uint32_t uKey, std::string guid, CFileExt& ext, std::string& outbuf);
	//某一个文件 接收完成
	static bool PackageReciveOver(uint32_t uKey, std::string& buf, std::string guid, std::string sessionid);
	//本次会话操作结束
	static bool PackageSessionOver(uint32_t uKey, std::string& buf, std::string sessionid);


};

