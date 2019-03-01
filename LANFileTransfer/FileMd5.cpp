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
		{//���ļ�ʧ��
			bResult = false;
			break;
		}
		fseek(pFile, 0, SEEK_END);
		MD5MSG md5msg;
		if ((md5msg.len = ftell(pFile)) == -1)//ftell��������long,���Ϊ2GB,��������-1
		{
			fclose(pFile);
			bResult = false;
			break;
		}

		rewind(pFile); //�ļ�ָ�븴λ���ļ�ͷ
		md5msg.A = 0x67452301;
		md5msg.B = 0xefcdab89;
		md5msg.C = 0x98badcfe;
		md5msg.D = 0x10325476; //��ʼ�����ӱ���
		md5msg.flen[1] = md5msg.len / 0x20000000;     //md5msg.flen��λ��bit
		md5msg.flen[0] = (md5msg.len % 0x20000000) * 8;
		memset(md5msg.x, 0, 64);   //��ʼ��md5msg.x����Ϊ0
		fread(&md5msg.x, 4, 16, pFile); //��4�ֽ�Ϊһ��,��ȡ16������
		for (md5msg.i = 0; md5msg.i < md5msg.len / 64; md5msg.i++)	    //ѭ������ֱ���ļ�����
		{
			md5(md5msg);
			memset(md5msg.x, 0, 64);
			fread(&md5msg.x, 4, 16, pFile);
		}
		((char*)md5msg.x)[md5msg.len % 64] = (char)128; //�ļ�������1,��0����,128�����Ƽ�10000000
		if (md5msg.len % 64 > 55)
		{
			md5(md5msg);
			memset(md5msg.x, 0, 64);
		}
		memcpy(md5msg.x + 14, md5msg.flen, 8);    //�ļ�ĩβ����ԭ�ļ���bit����
		md5(md5msg);
		fclose(pFile);

		char szMD5[64] = { 0 };
		sprintf(szMD5, "%08x%08x%08x%08x", PP(md5msg.A), PP(md5msg.B), PP(md5msg.C), PP(md5msg.D));//�ߵ�λ�淴���
		szmd5 = szMD5;	
	} while (false);
	return bResult;
}
