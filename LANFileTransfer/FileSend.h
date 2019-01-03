#pragma once
#include "FileExt.h"
#ifdef WIN32
#include "BaseFile.h"
#endif
#include "UtilString.h"
#include "file_open.h"
#include "TimeOut.h"
#define IPMSG_DEFAULT_IOBUFMAX		(4*1024*1024)	// 4MB
//#define IPMSG_DEFAULT_IOBUFMAX		100*1024	// 4MB
//文件发送对象
class CFileSend
{
public:
	CFileSend();
	~CFileSend();
	void SetFileExt(CFileExt* pext);
	CFileExt* GetFileExt();
	bool Start(bool bFlag);
	bool IsStart();
	void SetRelease(bool bRelease);
	bool IsRelease();
	void SetOffset(uint64_t uOffset);
	uint64_t GetOffSet();
	//设置当前发送文件的guiid
	void SetCurrentSendFile(std::string guid);
	//获取当前需要发送的文件
	std::string GetCurrentSendFile();
	//获取当前正在发送的文件的大小
	uint64_t GetCurrentSendFileSize();
	//获取该次对象总共发送的文件大小（大小包括目录已发送文件的大小）
	uint64_t GetObjectSendSize();
	//获取发送数据的buf 返回buf  ulen为返回buf的长度
	char* GetSendBuf(uint64_t& uLen);
	bool IsComplete();  //当前文件是否发送完毕
	void SetFileFlag(std::string guid, bool bFlag);
	void Clear();
	bool IsNotifyProgress();
private:
	bool m_bRelease;
	bool m_bStartFlag;  //是否已经开始传输标记
	uint64_t m_uOffSet;  //当前发送文件的偏移量
	uint64_t m_uCurrentFileSize;  //当前正在发送的文件的大小
	CFileExt m_Ext;
	//CBaseFile m_file; //操作文件对象
	FOpen m_file;
	std::string m_szFilePath;  //当前正在发送的文件路径
	std::string m_szGuid;  //当前发送文件对应的GUIID
	char* m_uBuff;  //文件数据buff的指针
	bool m_bNotifyProgress;
	CTimeOut  m_time;
};

