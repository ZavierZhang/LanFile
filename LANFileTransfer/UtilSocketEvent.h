#pragma once
#include <string>
namespace Util
{
	namespace SocketEvent
	{
		class CSocketEvent
		{
		public: 
			//����socket��������  ����socketkey
			virtual void OnSocketAccept(UINT32 uKey) = 0;
			//�ͻ������ӳɹ��ص�
			virtual void OnSocketConnect(UINT32 uKey,UINT32 uCode) = 0;
			//SOCKET ��������֮��Ļص� �ϲ�ͨ���˻ص��������  ���巢�Ͷ�������ײ�����
			virtual void OnSocketPreSend(UINT32 uKey,std::string& buf) = 0;
			//��������֮��Ļص�
			virtual void OnSocketEndSend() = 0;
			//����������֮ǰ�ص�
			virtual void OnSocketPreRecv(UINT32 uKey,UINT64& uLenBuff) = 0;
		    //����������֮��ص�  �������ݵ�buf
			virtual void OnSocketEndRecv(UINT32 uKey,std::string& buf) = 0;
			//�ر�����
			virtual void OnSocketClose(UINT32 uKey) = 0;
		};
	}
}
