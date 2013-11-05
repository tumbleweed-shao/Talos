#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <vector>

class NICIntroducer
{
public:
	char NICName[100];
	const char* NICPhysicalAdd;
	std::vector<std::string> GetNICAdapterInfo();
};