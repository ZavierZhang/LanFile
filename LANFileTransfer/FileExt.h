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
	uint64_t uFileSize;  //�ļ���С
	std::string filepath;  //�ļ�ȫ·��(����ʱΪ����ȫ·��������ʱ)
	std::string filename; //�ļ�����
	std::string filemd5;  //�ļ���MD5ֵ
	std::string guid;  //��ʾ��ǰ�ļ���guid  ��߲�ʹ��MD5 ��ֹ��ͬ���ļ���MD5ֻ�����ļ�������
	bool bOK;  //���ļ��Ƿ�����ϻ��߽������

};
std::string GetOneGuid();
class CFileExt
{
public:
	CFileExt();
	~CFileExt();
	//�ļ�ȫ·��
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
	//���ý�Ҫ���͵��ļ� pFxfolderĿ¼ǰ׺
	
	bool FindDirectoryAllFiles(std::wstring szPath,std::wstring strSourceFolder);
	//Unix ����Ŀ¼�������ļ�
	void scan_dir(const char *dir,const char* srcdir);
	void SetFileAllSize(uint64_t uFileSize);
	uint64_t GetFileAllSize();
	//��ȡ�ѳɹ������ļ��Ĵ�С
	uint64_t GetAlreadySendSize();
	//�ӿڽ��������ļ���Ϣ
	bool Parsing();
protected:
	uint64_t GetFileLength(std::wstring szFileName);
private:
	uint64_t m_uAllFileSize;  //�ļ��ܴ�С
	bool m_bIsDirectory;
	std::string m_szPath;  
	std::string m_szGUID; //��־��ǰ���ڴ����ļ���GUID
	std::string m_szSessionID; //��־�ỰID ��־һ���ļ�������ID
	std::string m_szFileName;  //�ļ�����  �����ļ��������ļ�������
	std::map<std::string,SignalFileObj> m_szFileList;  //�ļ���ʱ�����ļ��б� �ļ���guid ��Ӧ�ļ��ĵ����ļ���Ϣ
	bool m_bParsing;  //�Ƿ��Ѿ���ȡ����Ӧ���ļ���Ϣ
};

