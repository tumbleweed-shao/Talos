#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <iostream>
 

#include <stdlib.h>
#include <stddef.h>
#include <winioctl.h>
#include <vector>
#include <strsafe.h>
#include "HDD.h"
#define  MAX_IDE_DRIVES  16
void ErrorExit(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    //MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw); 
}  
 char * flipAndCodeBytes (char * str)   
{   
    static char flipped [1000];   
    int num = strlen (str);   
    strcpy (flipped, "");   
    for (int i = 0; i < num; i += 4)   
    {   
        for (int j = 1; j >= 0; j--)   
        {   
            int sum = 0;   
            for (int k = 0; k < 2; k++)   
            {   
                sum *= 16;   
                switch (str [i + j * 2 + k])   
                {   
                case '0': sum += 0; break;   
                case '1': sum += 1; break;   
                case '2': sum += 2; break;   
                case '3': sum += 3; break;   
                case '4': sum += 4; break;   
                case '5': sum += 5; break;   
                case '6': sum += 6; break;   
                case '7': sum += 7; break;   
                case '8': sum += 8; break;   
                case '9': sum += 9; break;   
                case 'a': sum += 10; break;   
                case 'b': sum += 11; break;   
                case 'c': sum += 12; break;   
                case 'd': sum += 13; break;   
                case 'e': sum += 14; break;   
                case 'f': sum += 15; break;   
                }   
            }   
            if (sum > 0)    
            {   
                char sub [2];   
                sub [0] = (char) sum;   
                sub [1] = 0;   
                strcat (flipped, sub);   
            }   
        }   
    }   
	 return flipped;
 }
std::vector<std::string> HDD::GetHDDSerialNumber ()
{
	std::vector<std::string> returnedvalue;
 int done = FALSE;
 int drive = 0;
 std::string result;
 std::vector<std::string> DriverSerialNos;
 
 for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
 {
  HANDLE hPhysicalDriveIOCTL = 0;
  char driveName [256];
  sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);
  hPhysicalDriveIOCTL = CreateFile (driveName, 0,FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,OPEN_EXISTING, 0, NULL);
  //ErrorExit(TEXT("NICAdapter"));
  if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
  {
   STORAGE_PROPERTY_QUERY query;
   DWORD cbBytesReturned = 0;
   char buffer [10000];
 
   memset ((void *) & query, 0, sizeof (query));
   query.PropertyId = StorageDeviceProperty;
   query.QueryType = PropertyStandardQuery;
 
   memset (buffer, 0, sizeof (buffer));
 
   if ( DeviceIoControl (hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY,
         & query,
         sizeof (query),
         & buffer,
         sizeof (buffer),
         & cbBytesReturned, NULL) )
   {         
    STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *) & buffer;
    char serialNumber [100];

	strcpy (serialNumber,    
                     flipAndCodeBytes ( & buffer [descrip -> SerialNumberOffset]));   
	DriverSerialNos.push_back( std::string( serialNumber ) );
   }
  }
  CloseHandle (hPhysicalDriveIOCTL);
 }
 return returnedvalue; 
}
