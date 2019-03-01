#ifndef _YZJ__COMMON__FILE__FILE_OPEN_H
#define _YZJ__COMMON__FILE__FILE_OPEN_H

#include <string>
#include <cstdio>

class FOpen
{
public:
	// 打开模式
	enum MODE
	{
		R_MODE_TYPE,
		R_PLUS_MODE_TYPE,
		R_B_MODE_TYPE,
		R_B_PLUS_MODE_TYPE,
		W_MODE_TYPE,
		W_PLUS_MODE_TYPE,
		W_B_MODE_TYPE,
		W_B_PLUS_MODE_TYPE,
		A_MODE_TYPE,
		A_PLUS_MODE_TYPE,
		A_B_MODE_TYPE,
		A_B_PLUS_MODE_TYPE,
	};

public:
	FOpen();

	virtual ~FOpen();

private:
	static std::string GetModeString(const MODE& mode);

	// 根据不同系统打开文件
	static FILE* OpenFileInDifferentOS(const std::string& path, const std::string mode);

public:
	bool Open(const std::string& path, const std::string mode);

	bool Open(const std::string& path, const MODE& mode);

	int create_file_directory(const char* pDir);

	int Close();

	bool IsOpen();

	long GetLength();

	long Tell();

	void Rewind();

	int FFlush();

	// 返回真实读取的项数，若大于count则意味着产生了错误。另外，产生错误后，文件位置指示器是无法确定的。
	// 若其他stream或buffer为空指针，或在unicode模式中写入的字节数为奇数，此函数设置errno为EINVAL以及返回0。
	size_t Read(void* buf, size_t element_size, size_t element_count);

	size_t ReadChar(char* buf, size_t element_count);

	// 返回实际写入数据项个数
	size_t Write(void* buf, size_t element_size, size_t element_count);

	size_t WriteChar(char* buf, size_t element_count);

private:
	FILE* file_ = nullptr;
};


#endif
