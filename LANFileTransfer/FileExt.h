#pragma once
#include<string>
#include <map>

struct SignalFileObj
{
	SignalFileObj()
	{
		uFileSize = 0;
		bOK = false;
	}
	uint64_t uFileSize;  //文件大小
	std::string filepath;  //文件全路径(发送时为本地全路径，接收时)
	std::string filename; //文件名字
	std::string filemd5;  //文件的MD5值
	std::string guid;  //表示当前文件的guid  这边不使用MD5 防止相同的文件，MD5只检验文件完整性
	bool bOK;  //该文件是否发送完毕或者接受完毕

};
std::string GetOneGuid();
class CFileExt
{
public:
	CFileExt();
	~CFileExt();
	//文件全路径
	void SetPath(const char* szPath);
	void SetDirName(const char* szFileName);
	std::string GetDirName();
	std::string GetPath();
	bool IsDirectory();
	void SetDirectory(bool bDir);
	std::map<std::string, SignalFileObj>& GetFileList();
	bool GetSignalFileObj(std::string szGuiid, SignalFileObj &obj);
	std::string GetFileName();
	std::string GetGUID();
	std::string GetSessionID();
	void SetSessionID(std::string sessionID);
	void SetFileFlag(bool bFlag,std::string guid);
	//设置将要发送的文件 pFxfolder目录前缀
	
	bool FindDirectoryAllFiles(std::wstring szPath,std::wstring strSourceFolder);
	//Unix 遍历目录下所有文件
	void scan_dir(const char *dir,const char* srcdir);
	void SetFileAllSize(uint64_t uFileSize);
	uint64_t GetFileAllSize();
	//获取已成功发送文件的大小
	uint64_t GetAlreadySendSize();
	//接口解析发送文件信息
	bool Parsing();
protected:
	uint64_t GetFileLength(std::wstring szFileName);
private:
	uint64_t m_uAllFileSize;  //文件总大小
	bool m_bIsDirectory;
	std::string m_szPath;  
	std::string m_szGUID; //标志当前正在传输文件的GUID
	std::string m_szSessionID; //标志会话ID 标志一次文件操作的ID
	std::string m_szFileName;  //文件名字  若是文件夹则是文件夹名字
	std::map<std::string,SignalFileObj> m_szFileList;  //文件夹时所有文件列表 文件的guid 对应文件的单个文件信息
	bool m_bParsing;  //是否已经读取过相应的文件信息
};

