#pragma once
#include "UtilClient.h"
#include <string>
//�ͻ���TCPģ��  ���ֱ��ʹ�ü򵥵��첽ѡ��ģ��
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
	bool Connect(const char* ip, int port,uint32_t& uKey);
	bool Send(std::string& buf);  //����buffer
	void Close();
	void SetFlag(int nFlag);
	int GetFlag();
protected:
	int m_nFlag;  //nFlag=1 ʱ��ʾ�����ļ�������
	Netfd m_fd;   //��ǰ���ӵ�fd
};

