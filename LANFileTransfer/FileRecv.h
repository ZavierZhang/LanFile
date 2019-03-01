#pragma once
#include"FileExt.h"
#include "TimeOut.h"
#include "file_open.h"
#ifdef WIN32
#include "BaseFile.h"
#endif // WIN32
//文件接收对象
class CFileRecv
{
public:
	CFileRecv();
	~CFileRecv();
	void SetFileExt(CFileExt* pext);
	bool Start(bool bFlag);
	void SetRelease(bool bRelease);
	bool IsRelease();
	void SetRecvBasePath(std::string path);
	bool IsStart();
	//设置当前发送文件的guiid
	void SetCurrentReciveFile(std::string guid);
	//获取当前需要发送的文件
	std::string GetCurrentReciveFile();
	//获取下一个要接受文件的guid 如果为空则表示完成
	std::string GetNextReciveGuid();
	std::string GetCurrentReciveGuid();
	std::string GetSessionID();
	CFileExt& GetFileExt();
	bool ReciveFile(std::string& databuf);
	bool CheckComplete(); //当前接收的文件是否完毕
	bool CheckMD5();  //检测MD5
	bool CheckSessionOK();   //检测当前文件传输是否结束
	void SetFileOkFlag(bool bFlag,std::string guid);
	void Clear();
	bool IsNotifyProgress();
	uint64_t GetAlreadyRecvSize();
private:
	bool m_bRelease;
	bool m_bStartFlag;  //是否已经开始传输标记
	uint64_t m_uOffSet;  //文件偏移量  当前已接收文件大小
	uint64_t m_uFileSize; //当前接收文件的大小
	CFileExt m_Ext;
	//CBaseFile m_file; //操作文件对象
	FOpen m_file;
	std::string m_szBaseFilePath;  //当前接收的基准路径  
	std::string m_szFilePath;   //当前正在接受文件的路径
	std::string m_szGuid;  //当前正在接受文件的guiid
	char* m_uBuff;  //文件数据buff的指针
	bool m_bNotifyProgress;
	CTimeOut  m_time;
};

