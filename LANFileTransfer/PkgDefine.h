#pragma once
#include <string>
enum SOCKET_CHANNEL
{
	CHANNEL_SEVER=1,    //服务端连接数据
	CHANNEL_CLIENT=2,  //客户端连接数据
};
enum PkgDataType
{
	PKG_JSON=1, //发送的json数据
	PKG_DATA=2, //发送的二进制流数据
};
//文件传输过程中的文件标志
enum FileSign
{
	FS_INIT = 0,
	FS_CLIENT_AGREEDIRFILE=1000,//client---->Sever  客户端同意接受文件
	FS_SEVER_FILE_INFO_LIST,//Sever---->client  客户端同意后服务端发送给客户端的文件列表
	FS_FILE_INFO,  //client---->Sever 收到服务端文件之后 客户端发送此命令请求某一个文件的具体信息
	FS_FILE_SUCCESS,
	FS_FILE_OVER,//一次传输结束
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
	std::string  m_buf;  //携带的数据
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