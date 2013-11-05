#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <vector>


class RLE{
public:
	std::string RLEEncode(std::string str);
	std::string m_replace(std::string str,std::string pattern,std::string dstPattern);
	};