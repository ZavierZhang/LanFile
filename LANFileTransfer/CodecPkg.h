#pragma once
#include "Pkg.h"
class CCodecPkg
{
public:
	CCodecPkg();
	~CCodecPkg();
	static bool MakePkg(CPkgCode& pKg, uint16_t uCmd, uint32_t uBufLen, uint8_t* buf);
	static bool ParsePkg(CPkgDecode& pkg, std::string& inbuf);
};

