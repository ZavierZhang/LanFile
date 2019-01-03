#pragma once
#include "UtilClient.h"
#include <string>
//客户端TCP模型  这边直接使用简单的异步选择模型
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
	bool Connect(const char* ip, int port,uint32_t& uKey);
	bool Send(std::string& buf);  //发送buffer
	void Close();
	void SetFlag(int nFlag);
	int GetFlag();
protected:
	int m_nFlag;  //nFlag=1 时表示接收文件数据了
	Netfd m_fd;   //当前链接的fd
};

