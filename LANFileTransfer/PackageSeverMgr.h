#pragma once
#include "PkgDefine.h"
#include"UtilFileEvent.h"
#include "Pkg.h"
#include "DecodeBuf.h"
//基于LibEvent 的服务端
#include "UtilSever.h"
//服务端以底层线程模式驱动上层 写入发送数据
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
	//以下事件 回调每一个连接的fd 对应一个线程 上层应注意这一点 若有共享数据 则需加锁保护
	//驱动写 
	virtual void OnFdWrite(Netfd fd);
	//驱动读
	virtual void OnFdRead(Netfd fd, const char* szBuf, int len);
	virtual void OnFdConnet(Netfd fd, int code);


protected:
	void DispatchPkg(uint32_t uKey, CPkgDecode& pKg);
private:
	Util::FILE_EVENT::CFileEvent* m_pSink;
	CDecodeBuf m_decode;

};

