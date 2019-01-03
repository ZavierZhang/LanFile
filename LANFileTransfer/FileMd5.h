#pragma once
#include <string>
class CMD5
{
public:
	CMD5();
	~CMD5();
	 int GetMd5(std::wstring szPath, std::string& szmd5);
};

