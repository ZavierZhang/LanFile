#include "stdafx.h"
#include "FileSendJsonData.h"


CFileSendJsonData::CFileSendJsonData()
{
}


CFileSendJsonData::~CFileSendJsonData()
{
}

//解析客户端同意接受文件的命令
bool CFileSendJsonData::ParseFileAgreeMsg(std::string& inBuf, std::string& sessionid)
{
	std::string strjson = inBuf;
	rapidjson::Document document;
	document.Parse(strjson.c_str());
	if (document.HasParseError())
	{
		return false;
	}
	else
	{
		sessionid=CTxRapidJson::ReadKeyValueString(document, "sessionid");
	}
	return true;
}
bool CFileSendJsonData::PackageFileList(CFileExt& ext, std::string& sessionid, std::string& outBuf)
{
	rapidjson::Document makedocument;
	rapidjson::Value arrayUsers(rapidjson::kArrayType);
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("sessionid", rapidjson::Value(sessionid.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	uint64_t uAllFileSize = 0;
	if (!ext.IsDirectory())
	{
		rapidjson::Value object1(rapidjson::kObjectType);
		uAllFileSize = ext.GetFileAllSize();
		std::string guid = ext.GetGUID();
		object1.AddMember("guid", rapidjson::Value(guid.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
		SignalFileObj obj;
		ext.GetSignalFileObj(guid, obj);
		object1.AddMember("md5", rapidjson::Value(obj.filemd5.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
		object1.AddMember("filename", rapidjson::Value(obj.filename.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
		object1.AddMember("fileSize", obj.uFileSize, makedocument.GetAllocator());
		arrayUsers.PushBack(object1, makedocument.GetAllocator());
	}
	else
	{
		for (std::map<std::string, SignalFileObj>::iterator iter = ext.GetFileList().begin(); iter != ext.GetFileList().end(); ++iter)
		{
			rapidjson::Value object2(rapidjson::kObjectType);
			object2.AddMember("md5", rapidjson::Value(iter->second.filemd5.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
			object2.AddMember("filename", rapidjson::Value(iter->second.filename.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
			object2.AddMember("fileSize", iter->second.uFileSize, makedocument.GetAllocator());
			object2.AddMember("guid", rapidjson::Value(iter->first.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
			uAllFileSize += iter->second.uFileSize;
			arrayUsers.PushBack(object2, makedocument.GetAllocator());
		}
	}
	object.AddMember("filename", rapidjson::Value(ext.GetFileName().c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	object.AddMember("fileSize", uAllFileSize, makedocument.GetAllocator());
	object.AddMember("isdir", ext.IsDirectory(), makedocument.GetAllocator());
	object.AddMember("fileslist", arrayUsers, makedocument.GetAllocator());
	outBuf = CTxRapidJson::JsonToString(object);
	return true;
}
//解析文件
bool CFileSendJsonData::ParseReqSendFile(std::string& inBuf, std::string& sessionid, std::string& guid, std::string& md5)
{
	std::string strjson = inBuf;
	rapidjson::Document document;
	document.Parse(strjson.c_str());
	if (document.HasParseError())
	{
		return false;
	}
	else
	{
		sessionid = CTxRapidJson::ReadKeyValueString(document, "sessionid");
		guid = CTxRapidJson::ReadKeyValueString(document, "guid");
		md5 = CTxRapidJson::ReadKeyValueString(document, "md5");
	}
	return true;
}
//解析一个文件文件完成命令buf
bool CFileSendJsonData::ParseCompleteSendFile(std::string& inBuf, std::string& sessionid, std::string& guid)
{
	std::string strjson = inBuf;
	rapidjson::Document document;
	document.Parse(strjson.c_str());
	if (document.HasParseError())
	{
		return false;
	}
	else
	{
		sessionid = CTxRapidJson::ReadKeyValueString(document, "sessionid");
		guid = CTxRapidJson::ReadKeyValueString(document, "guid");
	}
	return true;
}
