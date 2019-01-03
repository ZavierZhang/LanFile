#ifndef _YZJ__BUSINESS__MEETING__AUDIO__WINDOW_RAPIDJSON_H
#define _YZJ__BUSINESS__MEETING__AUDIO__WINDOW_RAPIDJSON_H

#include "rapidjson/reader.h"
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

class CTxRapidJson
{
public:
	static std::string JsonToString(const rapidjson::Value& valObj);
	static int ReadKeyValueInt(rapidjson::Value &object, const char*szKey, int iDefault = 0);
	static unsigned long long ReadKeyValueUint64(rapidjson::Value &object, const char*szKey, unsigned long iDefault = 0);
	static bool ReadKeyValueBool(rapidjson::Value &object, const char*szKey, bool bDefault = false);
	static std::string ReadKeyValueString(rapidjson::Value &object, const char*szKey, const char*szDefault = "");
	static bool ReadKeyRapidValue(rapidjson::Value &object, const char*szKey, rapidjson::Value &vDefault);
};
#endif 
