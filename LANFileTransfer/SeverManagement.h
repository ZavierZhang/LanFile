#pragma once
//#include "TcpSeverListen.h"
#include "FileSeverMgr.h"
#include "UtilLanFileTrans.h"
#include "UtilSever.h"
//�����ķ����  ��߱��������ֻ�Ὺһ����������CSeverManagement Ӧ��Ϊ����ģʽ
class CSeverManagement :public Util::LibEventSever::CBeginSeverRes
{
public:
	CSeverManagement();
	~CSeverManagement();
	//���������
	int Start(Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve* pSink);
	void Stop();  //ֹͣ����˹���

	void AddFile(const char*szGuiID, const char* szPath);
	bool CancelSend(const char*szSessionID);
	void OnBeginRes(bool bOk, int port);
	void RegisterSeverSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink);
private:
	bool m_bStart;
	Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve* m_pSink;
};

