#include "stdafx.h"
#include "UtilTool.h"
#include <string>
#ifdef WIN32
#include <objbase.h>
#else
#include <uuid/uuid.h> 
#endif

std::string Util::Tool::GetGuid()
{
#define GUID_LEN 64
		std::string szguid;
#ifdef WIN32
		char buffer[GUID_LEN] = { 0 };
		GUID guid;
		if (CoCreateGuid(&guid))
		{
			return szguid;
		}
		_snprintf_s(buffer, sizeof(buffer), "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2],
			guid.Data4[3], guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
		szguid = buffer;

#else 
		uuid_t uu;
		int i;
		uuid_generate(uu);
		for (i = 0; i < 16; i++)
		{
			char sz[4] = { 0 };
			sprintf(sz, "%02X-", uu[i]);
			szguid.append(sz);
		}
#endif
		return szguid;
}
