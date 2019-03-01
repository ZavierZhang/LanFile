#pragma once
#include"TcpClient.h"
#include <map>
#include "DecodeBuf.h"
#include"PkgDefine.h"
#include "UtilFileEvent.h"
#include "UtilClient.h"
class CClientPakcageMgr  : public Util::LibEventClient::EventClient::CEventClient
{
public:
	CClientPakcageMgr();
	~CClientPakcageMgr();
	//�߼��̵߳���
	void RegisterSink(Util::FILE_EVENT::CFileEvent* pSink);
	void  Connect(const char*ip, int port,uint32_t& uKey);
	void DisConnect(uint32_t& ukey);

	//�����̵߳���
	bool Send(uint32_t uKey, std::string& buf, FileSign sign= FS_INIT);
	void DispatchPkg(uint32_t uKey, CPkgDecode& pkg);
	//����ײ���� �߳̽��� ֪ͨ�ϲ�
	virtual void OnConnect(int uCode, Netfd fd);
	virtual void OnFdWrite(Netfd fd);
	virtual void OnFdRead(Netfd fd, const char* szBuf, int len);
private:
	std::map<uint32_t, CTcpClient*> m_Client; //�ͻ������лỰ��������
	CDecodeBuf m_decode;
	Util::FILE_EVENT::CFileEvent* m_pSink;
};

