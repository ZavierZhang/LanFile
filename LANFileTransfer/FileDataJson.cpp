#include "stdafx.h"
#include "FileDataJson.h"
#include "RapidJson.h"

std::string&   ReplaceStr(std::string&  str, const   std::string&  old_value, const  std::string&   new_value)
{
	while (true)
	{
		std::string::size_type   pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

CFileDataJson::CFileDataJson()
{
}


CFileDataJson::~CFileDataJson()
{
}

bool CFileDataJson::PackageAgreeFile(uint32_t uKey, std::string sessionid, std::string& outbuf)
{
	rapidjson::Document makedocument;
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("sessionid", rapidjson::Value(sessionid.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	outbuf = CTxRapidJson::JsonToString(object);
	return true;
}

bool CFileDataJson::ParseFileListPkg(uint32_t uKey, std::string& buf, CFileExt& ext)
{

	std::string strjson = buf;
	rapidjson::Document document;
	document.Parse(strjson.c_str());
	if (document.HasParseError())
	{
		return false;
	}
	else
	{
		std::string strSessionId = CTxRapidJson::ReadKeyValueString(document, "sessionid");
		std::string strFileName = CTxRapidJson::ReadKeyValueString(document, "filename");
		uint64_t allfileSize = CTxRapidJson::ReadKeyValueUint64(document, "fileSize");
		bool bIsDir= CTxRapidJson::ReadKeyValueBool(document, "isdir");
		//解析数组
		rapidjson::Value arrayFileList;
		CTxRapidJson::ReadKeyRapidValue(document, "fileslist",arrayFileList);
		bool bArray= arrayFileList.IsArray();
		for (rapidjson::SizeType nIdx = 0; nIdx < arrayFileList.Size(); nIdx++)
		{
			rapidjson::Value& obj = arrayFileList[nIdx];
			if (!obj.IsNull())
			{
				std::string filename = CTxRapidJson::ReadKeyValueString(obj, "filename");
				std::string strmd5 = CTxRapidJson::ReadKeyValueString(obj, "md5");
				std::string strGuid = CTxRapidJson::ReadKeyValueString(obj, "guid");
				uint64_t fileSize = CTxRapidJson::ReadKeyValueUint64(obj, "fileSize");
				SignalFileObj obj;
				obj.filemd5 = strmd5;
				//支持跨平台串
#ifdef WIN32
		std::string szTemp = ReplaceStr(filename, "/", "\\");
#else
		std::string szTemp = ReplaceStr(filename, "\\", "/");
#endif
				obj.filename = szTemp;
				obj.uFileSize = fileSize;
				ext.GetFileList().insert(make_pair(strGuid, obj));
			}
			ext.SetFileAllSize(allfileSize);
			ext.SetSessionID(strSessionId);
			ext.SetDirectory(bIsDir);
			ext.SetDirName(strFileName.c_str());
		}
	}
	return true;
}

bool CFileDataJson::PackageReqFileInfo(uint32_t uKey, std::string guid, CFileExt& ext, std::string& outbuf)
{
	rapidjson::Document makedocument;
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("sessionid", rapidjson::Value(ext.GetSessionID().c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	SignalFileObj obj;
	if (!ext.GetSignalFileObj(guid, obj))
	{
		//assert(false);
		return false;
	}
	object.AddMember("filename", rapidjson::Value(obj.filename.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	object.AddMember("md5", rapidjson::Value(obj.filemd5.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	object.AddMember("guid", rapidjson::Value(guid.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	object.AddMember("fileSize", obj.uFileSize, makedocument.GetAllocator());
	outbuf = CTxRapidJson::JsonToString(object);
	return true;
}

bool CFileDataJson::PackageReciveOver(uint32_t uKey, std::string& buf, std::string guid, std::string sessionid)
{
	rapidjson::Document makedocument;
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("sessionid", rapidjson::Value(sessionid.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	object.AddMember("guid", rapidjson::Value(sessionid.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	buf = CTxRapidJson::JsonToString(object);
	return true;
}

bool CFileDataJson::PackageSessionOver(uint32_t uKey, std::string& buf, std::string sessionid)
{


	rapidjson::Document makedocument;
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("sessionid", rapidjson::Value(sessionid.c_str(), makedocument.GetAllocator()).Move(), makedocument.GetAllocator());
	buf = CTxRapidJson::JsonToString(object);
	return true;
}