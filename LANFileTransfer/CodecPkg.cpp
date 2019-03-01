#include "stdafx.h"
#include "CodecPkg.h"
#include "Pkg.h"

CCodecPkg::CCodecPkg()
{
}


CCodecPkg::~CCodecPkg()
{
}
bool CCodecPkg::MakePkg(CPkgCode& pKg, uint16_t uCmd, uint32_t uBufLen, uint8_t* buf)
{
	std::string tempbuf;
	tempbuf.assign((const char*)buf, uBufLen);
	return pKg.PkgCode(uCmd, tempbuf);
}
bool CCodecPkg::ParsePkg(CPkgDecode& pkg, std::string& inbuf)
{
	uint32_t len = 0;
	if (!CPkgParse::PkgCheck(inbuf, len))
	{
		//assert(false);
		return	false;//·Ç·¨°ü
	}
	std::string outbuf;
	if (!CPkgParse::PkgRead(inbuf, outbuf, len))
	{
		return false;
	}
	pkg.PkgDecode(outbuf);
	inbuf.erase(0, len);
	return true;
}
