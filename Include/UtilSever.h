#pragma once
#include "ExportDefine.h"
//����libevent�ķ���˽ӿ�
namespace Util
{
	namespace LibEventSever
	{
		class CBeginSeverRes
		{
		public:
			virtual void OnBeginRes(bool bOk, int port) = 0;
		};
		class CEventSever
		{
		public:
			//����������
			virtual void OnAccept(Netfd fd) = 0;
			//�����¼� �ص�ÿһ�����ӵ�fd ��Ӧһ���߳� �ϲ�Ӧע����һ�� ���й������� �����������
			virtual void OnFdWrite(Netfd fd) = 0;
			virtual void OnFdRead(Netfd fd,const char* szBuf,int len) = 0;
			virtual void OnFdConnet(Netfd fd, int code) = 0;
		};
		 int Init(int port);
		 void Reg(Util::LibEventSever::CEventSever* pSink);
		 void RegBeginSever(Util::LibEventSever::CBeginSeverRes* pSink);
		 void BeginSever(bool bBlock=false);
		 void SetFdCallBack(Netfd fd,bool bCbRead, bool bCbWrite);
		 bool Send(Netfd fd, const char* buf, int len);
		 void Stop(Netfd fd);
		//ֹͣ��������
		 void EndSever();
		//�ͷ���Դ
		 void UnInit();
	}
}

