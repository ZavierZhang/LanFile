#include "stdafx.h"
#include "FileMd5.h"
#include <stdio.h>
#include "md5.h"
#include "UtilString.h"

CMD5::CMD5()
{

}
CMD5::~CMD5()
{

}

int CMD5::GetMd5(std::wstring szPath, std::string& szmd5)
{

	int bResult = true;
	do
	{
#ifdef WIN32
		FILE *pFile = _wfsopen(szPath.c_str(), L"rb", _SH_DENYNO);
#else
		FILE *pFile = fopen(CvtW2A(szPath).c_str(), "rb");
#endif
		
		if (NULL == pFile)
		{//打开文件失败
			bResult = false;
			break;
		}
		fseek(pFile, 0, SEEK_END);
		MD5MSG md5msg;
		if ((md5msg.len = ftell(pFile)) == -1)//ftell函数返回long,最大为2GB,超出返回-1
		{
			fclose(pFile);
			bResult = false;
			break;
		}

		rewind(pFile); //文件指针复位到文件头
		md5msg.A = 0x67452301;
		md5msg.B = 0xefcdab89;
		md5msg.C = 0x98badcfe;
		md5msg.D = 0x10325476; //初始化链接变量
		md5msg.flen[1] = md5msg.len / 0x20000000;     //md5msg.flen单位是bit
		md5msg.flen[0] = (md5msg.len % 0x20000000) * 8;
		memset(md5msg.x, 0, 64);   //初始化md5msg.x数组为0
		fread(&md5msg.x, 4, 16, pFile); //以4字节为一组,读取16组数据
		for (md5msg.i = 0; md5msg.i < md5msg.len / 64; md5msg.i++)	    //循环运算直至文件结束
		{
			md5(md5msg);
			memset(md5msg.x, 0, 64);
			fread(&md5msg.x, 4, 16, pFile);
		}
		((char*)md5msg.x)[md5msg.len % 64] = (char)128; //文件结束补1,补0操作,128二进制即10000000
		if (md5msg.len % 64 > 55)
		{
			md5(md5msg);
			memset(md5msg.x, 0, 64);
		}
		memcpy(md5msg.x + 14, md5msg.flen, 8);    //文件末尾加入原文件的bit长度
		md5(md5msg);
		fclose(pFile);

		char szMD5[64] = { 0 };
		sprintf(szMD5, "%08x%08x%08x%08x", PP(md5msg.A), PP(md5msg.B), PP(md5msg.C), PP(md5msg.D));//高低位逆反输出
		szmd5 = szMD5;	
	} while (false);
	return bResult;
}
