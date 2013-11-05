// Talos.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include "HDD.h"
int _tmain(int argc, _TCHAR* argv[])
{
	HDD hda;
	std::vector<std::string> SerailNam = hda.GetHDDSerialNumber();

	for(int i =0; i < SerailNam.size();i ++)
	{
		printf(SerailNam[i].c_str());
	}

	HINSTANCE hDll; //dll ¾ä±ú
	hDll=LoadLibrary(TEXT("Talos.Introducer.dll")); //¼ÓÔØdll
	if (hDll != NULL)
	{
	 typedef void (*TestIntroder)();
	 TestIntroder rle;
	 rle = (TestIntroder)GetProcAddress((HMODULE)hDll,"RunTestIntroder");
	 if (rle != NULL)
	 {
		 rle();
	 }
	return 0;
	}
}