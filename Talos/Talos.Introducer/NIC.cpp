// Talos.Introducer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NIC.h"
// Link with Iphlpapi.lib
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>


#pragma comment(lib, "IPHLPAPI.lib")

using namespace std;

#define WORKING_BUFFER_SIZE 15000
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


std::vector<std::string> NICIntroducer::GetNICAdapterInfo()
{
	std::vector<std::string> NIClists;
	
	// Declare and initialize variables 
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
	unsigned int i = 0;
	
    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_UNSPEC;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;

    char buff[100];
    DWORD bufflen=100;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

    family = AF_INET;//Get IPV4 

    //printf("Calling GetAdaptersAddresses function with family = ");
    //if (family == AF_INET)
    //    printf("AF_INET\n");
    //if (family == AF_INET6)
    //    printf("AF_INET6\n");
    //if (family == AF_UNSPEC)
    //    printf("AF_UNSPEC\n\n");

    // Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;

    do {

        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
        if (pAddresses == NULL) {
            printf
                ("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            exit(1);
        }

        dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            FREE(pAddresses);
            pAddresses = NULL;
        } else {
            break;
        }

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW));

    if (dwRetVal == NO_ERROR) {
		
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
		
		
        while (pCurrAddresses) {
			NICIntroducer eachIntroducer;
            //printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n",pCurrAddresses->Length);
            //printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
            //printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);

            /*pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast != NULL) {
                for (i = 0; pUnicast != NULL; i++)
                {
                    if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
                    {
                        sockaddr_in *sa_in = (sockaddr_in *)pUnicast->Address.lpSockaddr;
                        printf("\tIPV4:%s\n",inet_ntop(AF_INET,&(sa_in->sin_addr),buff,bufflen));
                    }
                    else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6)
                    {
                        sockaddr_in6 *sa_in6 = (sockaddr_in6 *)pUnicast->Address.lpSockaddr;
                        printf("\tIPV6:%s\n",inet_ntop(AF_INET6,&(sa_in6->sin6_addr),buff,bufflen));
                    }
                    else
                    {
                        printf("\tUNSPEC");
                    }
                    pUnicast = pUnicast->Next;
                }
                printf("\tNumber of Unicast Addresses: %d\n", i);
            } else
                printf("\tNo Unicast Addresses\n");
				
            pAnycast = pCurrAddresses->FirstAnycastAddress;
            if (pAnycast) {
                for (i = 0; pAnycast != NULL; i++)
                {
                    if (pAnycast->Address.lpSockaddr->sa_family == AF_INET)
                    {
                        sockaddr_in *sa_in = (sockaddr_in *)pAnycast->Address.lpSockaddr;
                        printf("\tIPV4:%s\n",inet_ntop(AF_INET,&(sa_in->sin_addr),buff,bufflen));
                    }
                    else if (pAnycast->Address.lpSockaddr->sa_family == AF_INET6)
                    {
                        sockaddr_in6 *sa_in6 = (sockaddr_in6 *)pAnycast->Address.lpSockaddr;
                        printf("\tIPV6:%s\n",inet_ntop(AF_INET6,&(sa_in6->sin6_addr),buff,bufflen));
                    }
                    else
                    {
                        printf("\tUNSPEC");
                    }
                    pAnycast = pAnycast->Next;
                }
                printf("\tNumber of Anycast Addresses: %d\n", i);
            } else
                printf("\tNo Anycast Addresses\n");
				
            pMulticast = pCurrAddresses->FirstMulticastAddress;
            if (pMulticast) {
                for (i = 0; pMulticast != NULL; i++)
                {
                    if (pMulticast->Address.lpSockaddr->sa_family == AF_INET)
                    {
                        sockaddr_in *sa_in = (sockaddr_in *)pMulticast->Address.lpSockaddr;
                        printf("\t IPV4:%s \n",inet_ntop(AF_INET,&(sa_in->sin_addr),buff,bufflen));
                    }
                    else if (pMulticast->Address.lpSockaddr->sa_family == AF_INET6)
                    {
                        sockaddr_in6 *sa_in6 = (sockaddr_in6 *)pMulticast->Address.lpSockaddr;
                        printf("\t IPV6:%s \n",inet_ntop(AF_INET6,&(sa_in6->sin6_addr),buff,bufflen));
                    }
                    else
                    {
                        printf("\tUNSPEC");
                    }
                    pMulticast = pMulticast->Next;
                }
                printf("\tNumber of Multicast Addresses: %d\n", i);
            } else
                printf("\tNo Multicast Addresses\n");

            pDnServer = pCurrAddresses->FirstDnsServerAddress;
            if (pDnServer) {
                for (i = 0; pDnServer != NULL; i++)
                {
                    printf("\tUnicast Address %d: %s\n", i, pDnServer->Address.lpSockaddr->sa_data);
                    pDnServer = pDnServer->Next;
                }
                printf("\tNumber of DNS Server Addresses: %d\n", i);
            } else
                printf("\tNo DNS Server Addresses\n");
				*/
            //printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
            //printf("\tDescription: %wS\n", pCurrAddresses->Description);
            //printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

            if (pCurrAddresses->PhysicalAddressLength != 0) {
                //printf("\tPhysical address: ");
				std::string dest;
                for (i = 0; i < (int) pCurrAddresses->PhysicalAddressLength;i++) 
                {
						char temp[5];
                        /*if (i == (pCurrAddresses->PhysicalAddressLength - 1))
                            printf("%.2X\n",(int) pCurrAddresses->PhysicalAddress[i]);
                        else
						{
                            printf("%.2X-",(int)pCurrAddresses->PhysicalAddress[i]);
							
						}*/
						sprintf(temp,"%.2X",(int)pCurrAddresses->PhysicalAddress[i]);//加个点能保持2位输出,前一位如果为空,写0
						dest.push_back(temp[0]);
						dest.push_back(temp[1]);
						if (i !=(pCurrAddresses->PhysicalAddressLength - 1)) dest.push_back('-');// 为了保持网卡地址格式,网卡值最后一个数后面不加-
						//memcpy(dest,(const char *)temp,strlen((const char *)temp) + 1);
                }
				std::wcstombs(eachIntroducer.NICName, pCurrAddresses->Description, 100);
				eachIntroducer.NICPhysicalAdd = dest.c_str();
				NIClists.push_back("NICInfo@");
				NIClists.push_back(eachIntroducer.NICName);
				NIClists.push_back(":");
				NIClists.push_back(eachIntroducer.NICPhysicalAdd);
            }
            //printf("\tFlags: %ld\n", pCurrAddresses->Flags);
            //printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
            //printf("\tIfType: %ld\n", pCurrAddresses->IfType);
            //printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
            //printf("\tIpv6IfIndex (IPv6 interface): %u\n",pCurrAddresses->Ipv6IfIndex);
            //printf("\tZoneIndices (hex): ");
            //for (i = 0; i < 16; i++)
            //    printf("%lx ", pCurrAddresses->ZoneIndices[i]);
            //printf("\n");

            //pPrefix = pCurrAddresses->FirstPrefix;
            //if (pPrefix) {
            //    for (i = 0; pPrefix != NULL; i++)
            //        pPrefix = pPrefix->Next;
            //    printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
            //} else
            //    printf("\tNumber of IP Adapter Prefix entries: 0\n");

            //printf("\n");
			//
            pCurrAddresses = pCurrAddresses->Next;
        }
		
    } 
	else {
        printf("Call to GetAdaptersAddresses failed with error: %d\n",
            dwRetVal);
        if (dwRetVal == ERROR_NO_DATA)
            printf("\tNo addresses were found for the requested parameters\n");
        else {

            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   
                // Default language
                (LPTSTR) & lpMsgBuf, 0, NULL)) {
                    printf("\tError: %s", lpMsgBuf);
                    LocalFree(lpMsgBuf);
                    if (pAddresses)
                        FREE(pAddresses);
                    exit(1);
            }
        }
    }
    if (pAddresses) {
        FREE(pAddresses);
    }
	return NIClists;
}

