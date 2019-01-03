#pragma once
#include "ExportDefine.h"
//����LibEvent �Ŀͻ��˽ӿ�
namespace Util
{
	namespace LibEventClient
	{
		namespace EventClient
		{
			class CEventClient
			{
			public:
				virtual void OnConnect(int uCode,Netfd fd) = 0;
				virtual void OnFdWrite(Netfd fd) = 0;
				virtual void OnFdRead(Netfd fd, const char* szBuf, int len) = 0;
			};
		}
		 void Reg(Util::LibEventClient::EventClient::CEventClient* pSink);
		 Netfd Connect(const char* ip, int port);
		 bool Send(Netfd fd, const char* buf, int len);
		 void DisConnect(Netfd fd);
		//�˳��ͻ�������ģ��  step1���ر����й��� ��ֹͣ�����߳�   step2���ͷ�����������Դ
		 void DoExit();
	}
}