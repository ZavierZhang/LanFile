#pragma once
#include <string>
enum SOCKET_CHANNEL
{
	CHANNEL_SEVER=1,    //�������������
	CHANNEL_CLIENT=2,  //�ͻ�����������
};
enum PkgDataType
{
	PKG_JSON=1, //���͵�json����
	PKG_DATA=2, //���͵Ķ�����������
};
//�ļ���������е��ļ���־
enum FileSign
{
	FS_INIT = 0,
	FS_CLIENT_AGREEDIRFILE=1000,//client---->Sever  �ͻ���ͬ������ļ�
	FS_SEVER_FILE_INFO_LIST,//Sever---->client  �ͻ���ͬ������˷��͸��ͻ��˵��ļ��б�
	FS_FILE_INFO,  //client---->Sever �յ�������ļ�֮�� �ͻ��˷��ʹ���������ĳһ���ļ��ľ�����Ϣ
	FS_FILE_SUCCESS,
	FS_FILE_OVER,//һ�δ������
	FS_FILE_CANCLE
};

class CPkgData
{
public:
	CPkgData()
	{
		SetPkgType(PKG_DATA);
	}
	void SetPkgType(PkgDataType type) { m_Type = type; }
	PkgDataType GetPkgType() { return m_Type; }
	void SetBuf(std::string& buf) { m_buf = buf; }
	std::string& GetBuf() { return m_buf; }
private:
	PkgDataType m_Type;
	std::string  m_buf;  //Я��������
};


struct ProgInfo
{
	ProgInfo()
	{
		uPos = 0;
		uFileSize = 0;
	}
	std::string szSessionID;
	uint64_t   uPos;
	uint64_t uFileSize;
};

struct ProgResInfo
{
	std::string szSessionID;
	uint32_t uCode;
};

#define  WM_PROGRESS   WM_USER+1
#define  WM_RESULT        WM_USER+2