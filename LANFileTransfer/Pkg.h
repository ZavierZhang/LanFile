#pragma once
#include <string>

#ifdef WIN32
#include<WinSock2.h>
#endif // WIN32
//1 �ֽڶ���
#ifdef _WIN32
#pragma pack( push, 1)
#else
#pragma pack(1)
#endif
struct PkgHeader
{
	uint32_t uPackageSize;   //����
	uint32_t  uPackageFlag;   //��־λ
	uint16_t uPackageSeq;   //���к�
	uint16_t uCommand; // ����ֵ
	uint16_t  uVersion;    //�汾
	uint16_t    uReversed;  // ����,���ļ���������Ϊ��һ����ĳ���
	inline void	Htonl()
	{
		uPackageSize = (uint32_t)htonl((u_long)uPackageSize);
		uPackageFlag = (uint32_t)htonl((u_long)uPackageFlag);
		uVersion = (uint16_t)htonl((u_long)uVersion);
		uCommand = (uint16_t)htons((u_short)uCommand);
		uReversed= (uint16_t)htons((u_short)uReversed);
	}
	inline void Ntohl()
	{
		uPackageSize = (uint32_t)ntohl((u_long)uPackageSize);
		uPackageFlag = (uint32_t)ntohl((u_long)uPackageFlag);
		uVersion = (uint16_t)ntohl((u_long)uVersion);
		uCommand = (uint16_t)ntohs((u_short)uCommand);
		uReversed = (uint16_t)ntohs((u_short)uReversed);
	}
};

#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif

class CPkg
{
public:
	CPkg() {}
	~CPkg() {}
	void SetCmd(uint16_t uCmd) { m_header.uCommand = uCmd; }
	void SetPkgSize(uint32_t uSize) { m_header.uPackageSize = uSize; }
	void SetSeq(uint16_t uSeq) { m_header.uPackageSeq = uSeq; }
	void SetFlag(uint32_t uFlag) { m_header.uPackageFlag = uFlag; }
	void SetVersion(uint16_t uVersion) { m_header.uVersion = uVersion; }
	void SetReversed(uint16_t uReversed) { m_header.uReversed = uReversed; }
	void SetBuf(std::string& buf) { m_buf = buf; }

	uint32_t GetPkgSize() { return m_header.uPackageSize; }
	uint16_t GetCmd() { return m_header.uCommand; }
	uint32_t GetFlag() { return m_header.uPackageFlag; }
	uint16_t GetVersion() { return m_header.uVersion; }
	uint16_t GetReversed() { return m_header.uReversed; }
	uint16_t GetSeq() { return m_header.uPackageSeq; }
	std::string& GetBuf() { return m_buf; }
	std::string GetHeaderBuf() 
	{
		m_header.Htonl();
		std::string buf;
		buf.assign((char*)&m_header, sizeof(m_header));
		return buf;
	}
private:
	PkgHeader m_header;  //���ݰ�ͷ
	std::string  m_buf;   //���ݰ���
};

class CPkgCode : public CPkg
{
public:
	bool PkgCode(uint16_t uCmd,std::string& inbuf);
	std::string& GetCodeBuf();
private:
	std::string m_Codebuf;  //���֮�������buf
};

class CPkgDecode : public CPkg
{
public:
	bool PkgDecode(std::string& inbuf);
};

class CPkgParse
{
public:
	//����ͷ
	static bool PkgCheck(std::string& inbuf, uint32_t& uPkgLen);
	//��ȡ������
	static bool PkgRead(std::string& inbuf, std::string& outPkg, uint32_t uPkgLen);
};

