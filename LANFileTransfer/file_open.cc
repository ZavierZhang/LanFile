#include "file_open.h"
#include "UtilString.h"
#ifdef _WIN32  
#include <direct.h>  
#include <io.h>  
#else
#include <stdarg.h>  
#include <sys/stat.h>
#include <stdlib.h>
#include<unistd.h>
#endif  

#ifdef _WIN32  
#define ACCESS _access  
#define MKDIR(a) _mkdir((a))  
#else 
#define ACCESS access  
#define MKDIR(a) mkdir((a),0755)  
#endif 

FOpen::FOpen()
{

}

FOpen::~FOpen()
{
	Close();
}

std::string FOpen::GetModeString(const FOpen::MODE& mode)
{
	switch (mode)
	{
	case FOpen::R_MODE_TYPE:
		return u8"r";
	case FOpen::R_PLUS_MODE_TYPE:
		return u8"r+";
	case FOpen::R_B_MODE_TYPE:
		return u8"rb";
	case FOpen::R_B_PLUS_MODE_TYPE:
		return u8"rb+";
	case FOpen::W_MODE_TYPE:
		return u8"w";
	case FOpen::W_PLUS_MODE_TYPE:
		return u8"w+";
	case FOpen::W_B_MODE_TYPE:
		return u8"wb";
	case FOpen::W_B_PLUS_MODE_TYPE:
		return u8"wb+";
	case FOpen::A_MODE_TYPE:
		return u8"a";
	case FOpen::A_PLUS_MODE_TYPE:
		return u8"a+";
	case FOpen::A_B_MODE_TYPE:
		return u8"ab";
	case FOpen::A_B_PLUS_MODE_TYPE:
		return u8"ab+";
	default:
		return u8"r";
	}
}

FILE* FOpen::OpenFileInDifferentOS(const std::string& path, const std::string mode)
{
#ifdef _WIN32
	return _wfopen(CvtA2W(path).c_str(), CvtA2W(mode).c_str());
#else
	return fopen(path.c_str(), mode.c_str());
#endif
}

bool FOpen::Open(const std::string& path, const std::string mode)
{
	if (file_)
	{
		Close();
	}
	file_ = OpenFileInDifferentOS(path, mode);
	return file_ != nullptr;
}

bool FOpen::Open(const std::string& path, const FOpen::MODE& mode)
{
	return Open(path, GetModeString(mode));
}

int FOpen::Close()
{
	int result = 0;
	if (file_)
	{
		result = fclose(file_);
		file_ = nullptr;
	}
	return result;
}

bool FOpen::IsOpen()
{
	return file_ != nullptr;
}

long FOpen::GetLength()
{
	long current = ftell(file_);
	long length = 0;
	fseek(file_, 0, SEEK_END);
	length = ftell(file_);
	fseek(file_, current, SEEK_SET);
	return length;
}

long FOpen::Tell()
{
	return ftell(file_);
}

void FOpen::Rewind()
{
	rewind(file_);
}

int FOpen::FFlush()
{
	return fflush(file_);
}

size_t FOpen::Read(void* buf, size_t element_size, size_t element_count)
{
	if (NULL == file_)
		return 0;
	return fread(buf, element_size, element_count, file_);
}

size_t FOpen::ReadChar(char* buf, size_t element_count)
{
	return Read(buf, sizeof(char), element_count);
}

size_t FOpen::Write(void* buf, size_t element_size, size_t element_count)
{
	if (NULL == file_)
		return 0;
	return fwrite(buf, element_size, element_count, file_);
}

size_t FOpen::WriteChar(char* buf, size_t element_count)
{
	return Write(buf, sizeof(char), element_count);
}
int FOpen::create_file_directory(const char* sPathName)
{
#ifdef WIN32
	return 0;
#else
	char DirName[256];
	int i, len;
	strcpy(DirName, sPathName);
	len = strlen(DirName);
	if ('/' != DirName[len - 1])
	{
		strcat(DirName, "/");
		len++;
	}
	for (i = 1; i < len; i++)
	{
		if ('/' == DirName[i])
		{
			DirName[i] = '\0';
			if (access(DirName, F_OK) != 0)
			{
				if (mkdir(DirName, 0777) == -1)
				{
					return 0;
				}
			}
			DirName[i] = '/';
		}
	}
	return 1;
#endif
}