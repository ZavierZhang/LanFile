#pragma once
#include "PkgDefine.h"
#include"UtilFileEvent.h"
#include "Pkg.h"
#include "DecodeBuf.h"
//����LibEvent �ķ����
#include "UtilSever.h"
//������Եײ��߳�ģʽ�����ϲ� д�뷢������
class CPackageSeverMgr : public Util::LibEventSever::CEventSever
{
public:
	CPackageSeverMgr();
	~CPackageSeverMgr();
	void RegisterSink(Util::FILE_EVENT::CFileEvent* pSink);
	bool SendFileCmd(uint32_t uKey,FileSign cmd,std::string& buf);
	uint32_t SendFileData(uint32_t uKey, std::string& buf);
	void Close(uint32_t uKey);

	virtual void OnAccept(Netfd fd);
	//�����¼� �ص�ÿһ�����ӵ�fd ��Ӧһ���߳� �ϲ�Ӧע����һ�� ���й������� �����������
	//����д 
	virtual void OnFdWrite(Netfd fd);
	//������
	virtual void OnFdRead(Netfd fd, const char* szBuf, int len);
	virtual void OnFdConnet(Netfd fd, int code);


protected:
	void DispatchPkg(uint32_t uKey, CPkgDecode& pKg);
private:
	Util::FILE_EVENT::CFileEvent* m_pSink;
	CDecodeBuf m_decode;

};

