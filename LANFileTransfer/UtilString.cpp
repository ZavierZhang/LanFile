#include "stdafx.h"
#include "UtilString.h"
#include <codecvt>

#ifdef WIN32
#else
#include <locale>
#endif // WIN32


//WIDE--->UTF8
std::string CvtW2A(const std::wstring &strValue)
{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(strValue);
}

//UTF8-->WIDE
std::wstring CvtA2W(const std::string & strValue)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(strValue);
}
