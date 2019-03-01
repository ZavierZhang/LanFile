#pragma once
//#ifndef LAN_FILE_API_OPTIN	//外部使用者请定义，不可重入
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
				virtual void OnPorgress(const char* szSessionID,uint64_t uPos,uint64_t uTotal)=0;  // 进度回调接口
				//uCode =0 表示正常结束 
				virtual void OnResult(const char* szSessionID, uint32_t uCode) = 0;   //结果数据回调
			};
			class CNetCheckEvent
			{
			public:
				virtual void OnCanLanFile(uint32_t uKey,const char* ip,bool bCan) = 0;
			};
		}

		//作为服务端接口  服务端为文件发送端
		namespace Sever
		{
			//初始化 服务端方  返回 可用的链接端口
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
			//作为服务端一方调用 这边上层仅仅是把信息填充 真正发送文件 需要等待 客户端获取操作才触发文件发送操作
			 void SendFile(const char*szSessionID,const char* szPath);
			 bool CancelSend(const char*szSessionID);
			 void RegisterSeverSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink);
			
		}
		//作为客户端接口   客户端为文件接收方
		namespace Client
		{
			//是否满足局域网传输条件  底层会记住 已经可用的链接 返回1 则表示直接可用  0 失败  2 等待结果
			  uint32_t CanLanFile(const char* ip, int nPort);
			//当前一次操作的sessionid
			 bool ReciveFile(const char*szSessionID, const char* szFilePath, const char* ip, int nPort);
			 bool CancelRecive(const char*szSessionID);  //取消接收
			 void RegisterClientSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent* pSink);
			 void RegisterCheckNetSink(Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent* pSink);
			 void DoClientExit();
		}
	}

	namespace OneGuid
	{
		//获取Guid
		std::string  GetOneGuid();
	}
}

  