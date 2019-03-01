#pragma once
#include "Pkg.h"
#include <map>
#include "AutoLock.h"
class CDecodeBuf
{
public:
	CDecodeBuf();
	~CDecodeBuf();
	bool AppendBuf(uint32_t uKey, std::string& buf);
	bool GetUsePkg(uint32_t uKey, CPkgDecode& pkg);
private:
	LockHelper m_LockBuf;
	std::map<uint32_t, std::string> m_Buf;  //服务端收到包的buf
};

