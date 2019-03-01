#include "stdafx.h"
#include "Pkg.h"
#define  MIN_PACKAGE_SIZE   8
#define  MINI_PRO_FLAG1		0x12345678
bool CPkgCode::PkgCode(uint16_t uCmd, std::string& inbuf)
{
	SetCmd(uCmd);
	SetSeq(0);
	SetVersion(1);
	SetFlag(MINI_PRO_FLAG1);
	SetReversed(0);
	SetPkgSize(inbuf.length() + sizeof(PkgHeader));
	std::string buf=GetHeaderBuf();
	m_Codebuf = buf + inbuf;
	return true;
}
std::string& CPkgCode::GetCodeBuf()
{
	return m_Codebuf;
}



bool CPkgDecode::PkgDecode(std::string& inbuf)
{
	std::string temp;
	temp.assign(inbuf);
	const char* pkg = temp.c_str();
	PkgHeader* pHeader = (PkgHeader*)pkg;
	pHeader->Ntohl();
	SetCmd(pHeader->uCommand);
	SetSeq(pHeader->uPackageSeq);
	SetVersion(pHeader->uVersion);
	SetFlag(pHeader->uPackageFlag);
	SetReversed(pHeader->uReversed);
	uint32_t uSize=pHeader->uPackageSize;
	SetPkgSize(uSize);
	temp.erase(0, sizeof(PkgHeader));
	SetBuf(temp);
	return true;
}


bool CPkgParse::PkgCheck(std::string& inbuf, uint32_t& uPkgLen)
{
	int nBuffLen = inbuf.length();
	if (nBuffLen < MIN_PACKAGE_SIZE)
	{
		//assert(false);
		return false;
	}
	std::string temp;
	temp.assign(inbuf);
	const char* pkg = temp.c_str();

	uint32_t uIndex = 4;
	for (; uIndex < (uint32_t)(nBuffLen - 4); uIndex++)
	{
		uint32_t uFlag = *(uint32_t*)(&pkg[uIndex]);
		uFlag =ntohl(uFlag);
		//找到了标志位
		if (MINI_PRO_FLAG1 == uFlag)
		{
			uint32_t uSize = *(uint32_t*)&pkg[uIndex - 4];
			uPkgLen = ntohl(uSize);  // 包大小
			return true;
		}
	}
	return false;
}
//读取包数据
bool CPkgParse::PkgRead(std::string& inbuf, std::string& outPkg, uint32_t uPkgLen)
{
	if (0 == uPkgLen || uPkgLen < MIN_PACKAGE_SIZE || inbuf.length()<uPkgLen)
		return false;
	outPkg =inbuf.substr(0, uPkgLen);
	return true;
}