#include "RapidJson.h"
//此函数转对象
std::string CTxRapidJson::JsonToString(const rapidjson::Value& valObj)
{
	rapidjson::StringBuffer sbBuf;
	rapidjson::Writer<rapidjson::StringBuffer> jWriter(sbBuf);
	valObj.Accept(jWriter);
	return std::string(sbBuf.GetString());
}

int CTxRapidJson::ReadKeyValueInt(rapidjson::Value &object, const char*szKey, int iDefault/* = 0*/)
{
	if (object.HasMember(szKey))
	{
		rapidjson::Value& vjson = object[szKey];
		if (vjson.IsInt())
		{
			int iVal = vjson.GetInt();
			return iVal;
		}
	}
	return iDefault;
}
unsigned long long CTxRapidJson::ReadKeyValueUint64(rapidjson::Value &object, const char*szKey, unsigned long iDefault/* = 0*/)
{
	if (object.HasMember(szKey))
	{
		rapidjson::Value& vjson = object[szKey];
		if (vjson.IsUint64())
		{
			unsigned long long iVal = vjson.GetUint64();
			return iVal;
		}
	}
	return iDefault;
}
bool CTxRapidJson::ReadKeyValueBool(rapidjson::Value &object, const char*szKey, bool bDefault/* = false*/)
{
	if (object.HasMember(szKey))
	{
		rapidjson::Value& vjson = object[szKey];
		if (vjson.IsBool())
		{
			bool bVal = vjson.GetBool();
			return bVal;
		}
	}
	return bDefault;
}
std::string CTxRapidJson::ReadKeyValueString(rapidjson::Value &object, const char*szKey, const char *szDefault/* = ""*/)
{
	if (object.HasMember(szKey))
	{
		rapidjson::Value& vjson = object[szKey];
		if (vjson.IsString())
		{
			std::string strVal = vjson.GetString();
			return strVal;
		}
	}
	return szDefault;
}
bool CTxRapidJson::ReadKeyRapidValue(rapidjson::Value &object, const char*szKey, rapidjson::Value &vDefault)
{
	if (object.HasMember(szKey))
	{
		vDefault = object[szKey];
		return true;
	}
	return false;
}