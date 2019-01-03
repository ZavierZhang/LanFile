#pragma once
//#ifndef LAN_FILE_API_OPTIN	//�ⲿʹ�����붨�壬��������
//#define LAN_FILE_API __declspec(dllexport)
//#else
//#define LAN_FILE_API __declspec(dllimport)
//#endif
#include <stdint.h>
#include <string>
namespace Util
{
	namespace LAN_FILE
	{

		void InitModule();
		void UnInitModule();
		namespace FILE_TRANSFER_EVENT
		{
			class CFileTransferEvent
			{
			public:
				virtual void OnPorgress(const char* szSessionID,uint64_t uPos,uint64_t uTotal)=0;  // ���Ȼص��ӿ�
				//uCode =0 ��ʾ�������� 
				virtual void OnResult(const char* szSessionID, uint32_t uCode) = 0;   //������ݻص�
			};
			class CNetCheckEvent
			{
			public:
				virtual void OnCanLanFile(uint32_t uKey,const char* ip,bool bCan) = 0;
			};
		}

		//��Ϊ����˽ӿ�  �����Ϊ�ļ����Ͷ�
		namespace Sever
		{
			//��ʼ�� ����˷�  ���� ���õ����Ӷ˿�
			namespace BeginSeverEve
			{
				class CBeginSeverEve
				{
				public:
					virtual void OnBeginSever(bool bOK, int Port) = 0;
				};
			}
			  int Init(Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve* pSink);
			  void UnInit();
			//��Ϊ�����һ������ ����ϲ�����ǰ���Ϣ��� ���������ļ� ��Ҫ�ȴ� �ͻ��˻�ȡ�����Ŵ����ļ����Ͳ���
			 void SendFile(const char*szSessionID,const char* szPath);
			 bool CancelSend(const char*szSessionID);
			 void RegisterSeverSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink);
			
		}
		//��Ϊ�ͻ��˽ӿ�   �ͻ���Ϊ�ļ����շ�
		namespace Client
		{
			//�Ƿ������������������  �ײ���ס �Ѿ����õ����� ����1 ���ʾֱ�ӿ���  0 ʧ��  2 �ȴ����
			  uint32_t CanLanFile(const char* ip, int nPort);
			//��ǰһ�β�����sessionid
			 bool ReciveFile(const char*szSessionID, const char* szFilePath, const char* ip, int nPort);
			 bool CancelRecive(const char*szSessionID);  //ȡ������
			 void RegisterClientSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink);
			 void RegisterCheckNetSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent* pSink);
			 void DoClientExit();
		}
	}

	namespace OneGuid
	{
		//��ȡGuid
		std::string  GetOneGuid();
	}
}

  