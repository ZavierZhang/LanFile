#include "stdafx.h"
#include "DecodeBuf.h"
#include "CodecPkg.h"

CDecodeBuf::CDecodeBuf()
{
}


CDecodeBuf::~CDecodeBuf()
{
}
bool CDecodeBuf::AppendBuf(uint32_t uKey, std::string& buf)
{
	CAutoLock lock(m_LockBuf);
	if (0==buf.length())
	{
		//assert(false);
		return false;
	}
	if (m_Buf.find(uKey)!=m_Buf.end())  //ÕÒµ½ÁË
	{
		m_Buf[uKey].append(buf);  //×·¼Óbuf
	}
	else
	{
		m_Buf[uKey] = buf;
	}
	return true;
}

bool CDecodeBuf::GetUsePkg(uint32_t uKey, CPkgDecode& pkg)
{
	CAutoLock lock(m_LockBuf);
	std::string& buf = m_Buf[uKey];
	if (buf.length() <= 0)
	{
		return false;
	}
	return CCodecPkg::ParsePkg(pkg, buf);
}