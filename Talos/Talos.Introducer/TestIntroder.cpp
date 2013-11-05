#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <strsafe.h>
#include "RLE.h"
#include "NIC.h"
#include "HDD.h"
#include "TestIntroder.h"


void TestIntroder::RunTestIntroder()
{
	NICIntroducer nicin;
	RLE rrle;
	std::string temp("abc");
	std::vector<std::string> inc =  nicin.GetNICAdapterInfo();
	printf("=============================\n");
	printf("====Get the NIC infomation===\n");
	
	for (int i = 0; i <inc.size();i++)
	{
		printf(inc[i].c_str());
		
		if (i % 4 ==3)
		{
			printf("\nNIC Address\n");
			printf("^^^^^^^^^^^^^^^^^^^^^\n");
		
			temp = rrle.m_replace(inc[i],"-","");

			std::string afterrle = rrle.RLEEncode(temp);
			printf("\n^^NIC RLE result ->Following^^\n");
			printf(afterrle.c_str());
			printf("\n");
		}
	}
	printf("=============================\n");
	printf("====Get the HDD infomation===\n");
	HDD hddd;
	std::vector<std::string> SerailNam = hddd.GetHDDSerialNumber();
	printf("%d",SerailNam);
	printf("\n");
	system("PAUSE");
	
}