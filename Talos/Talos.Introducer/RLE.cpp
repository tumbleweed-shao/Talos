
#include "stdafx.h"
// Link with Iphlpapi.lib
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <strsafe.h>
#include "RLE.h"

std::string RLE::RLEEncode(std::string inputStr)
{
	int length,number;	   			// length��ʾ����ĳ��ȣ�number��ʾ��������
	int count=0;							//count���ڼ�¼����
	std::string returnedstring;
	int TempNumOne=inputStr.size();
	char inputChars[30];//���ڱ�����������
	for (int a=0;a<=TempNumOne;a++)
    {
        inputChars[a]=inputStr[a];
    }
	length = strlen(inputChars);						//���������ݵĳ��ȸ���length
	for(int k=0,number=inputChars[0];k<length;k++)			//ѭ��������ѹ��
	{
		char atemp[2]; //�����洢��һλ����  AABC33R55   2A1B1C231R25
		char btemp[1]; //�����洢�ڶ�λ�ַ�  AABC33R55   2A1B1C231R25
		if(inputChars[k+1]==inputChars[k])						//���������������ͬ
		{
			number = inputChars[k];		//�����ݸ���number
			count++;			//������1������ѭ��
		} 
		else
		{ 	/*��������������ȣ������֮ǰ��ѹ�����룬���Ѳ��ȵ�����ֵ����number������count��0�������ж�֮�������*/
			sprintf(atemp,"%d",count+1); 
			sprintf(btemp,"%c",number); 
			//TODO: Just a Unit test start
			/*printf(atemp);
			printf(btemp);*/
			//TODO: Just a Unit test end
			returnedstring.push_back(atemp[0]);
			returnedstring.push_back(btemp[0]);
			number = inputChars[k+1];
			count=0;
		}
	} 
	
	return returnedstring;
}

//Replace string with the destPatrern
std::string RLE::m_replace(std::string str,std::string pattern,std::string dstPattern)
{
	int count=-1;
    std::string retStr="";
	std::string::size_type pos;

    int szStr=str.length();
    int szPattern=pattern.size();
    int i=0;
    int l_count=0;
    if(-1 == count) // replace all
        count = szStr;
    for(i=0; i<szStr; i++)
    {
        pos=str.find(pattern,i);
        if(std::string::npos == pos)
            break;
        if(pos < szStr)
        {
            std::string s=str.substr(i,pos-i);
            retStr += s;
            retStr += dstPattern;
            i=pos+pattern.length()-1;
            if(++l_count >= count)
            {
                i++;
                break;
            }
        }
    }
    retStr += str.substr(i);
    return retStr;
}