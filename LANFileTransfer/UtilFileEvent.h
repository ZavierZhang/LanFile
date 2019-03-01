#pragma once
#include"PkgDefine.h"
#include <string>
namespace Util
{
	namespace FILE_EVENT
	{
		class CFileEvent
		{
		public:
			//�ļ�ͨѶ����
			virtual void OnFileCmd(FileSign cmd, uint32_t uKey, std::string& buf) = 0;
			//�ļ����ͻص�  �����ʹ��
			virtual void OnFileSend(uint32_t uKey)=0;
			//�ļ����ջص� �ͻ���ʹ��
			virtual void OnFileRecive(uint32_t uKey, std::string& buf) = 0;
			//�ײ������쳣�� 
			virtual void OnFileError(uint32_t uKey, uint32_t uCode) = 0;
		};
	}
}
