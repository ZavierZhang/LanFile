#pragma once
#include "ExportDefine.h"
namespace Util
{
	namespace LibEventConnTest
	{
		namespace LibEventConnTestEve
		{
			class CLibEventConnTestEve
			{
			public:
				virtual void OnConnect(Netfd fd,bool bOk) = 0;
			};
		}
		 void Reg(Util::LibEventConnTest::LibEventConnTestEve::CLibEventConnTestEve* pSink);
		 Netfd ConnTest(const char*ip, int port);
		 void DoExit();
	}
}

