#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "ShellAPI.h"
#include <iostream>
using namespace std;

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

int main()
{
	cout << "This is KLIENT and klient will send SOMETHING to server\n";
 
    WORD ver = MAKEWORD(2,2);
    WSADATA wsaData;
    int retVal=0;
 
    WSAStartup(ver,(LPWSADATA)&wsaData);
 
    LPHOSTENT hostEnt;
 
    /// Collect information of the server
    hostEnt = gethostbyname("");
    if(!hostEnt)
    {
        MessageBox(NULL, L"Unable to collect gethostbyname()", L"ERROR", MB_OK);
        WSACleanup();
        return 1;
    }
 
    /// Creating a socket
    SOCKET clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 
    if(clientSock == SOCKET_ERROR)
    {
        MessageBox(NULL, L"Unable to create socket", L"ERROR", MB_OK);
        WSACleanup();
        return 1;
    }
 
    SOCKADDR_IN serverInfo;
 
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
 
    serverInfo.sin_port = htons(1111);
 
    retVal=connect(clientSock,(LPSOCKADDR)&serverInfo, sizeof(serverInfo));
    if(retVal==SOCKET_ERROR)
    {
        MessageBox(NULL, L"Unable to connect()", L"ERROR", MB_OK);
        WSACleanup();
        return 1;
    }
 
    MessageBox(NULL, L"Connection made sucessfully", L"SUCCESS", MB_OK);
 
    char *pBuf;
	char temp[512];
	cin.getline(temp,512);
	pBuf = new char [strlen(temp)];
	strcpy(pBuf,temp);
	//cout << pBuf << endl;
    MessageBox(NULL, L"Sending request from client", L"SENDING....", MB_OK);
    retVal = send(clientSock, pBuf, strlen(pBuf), 0);
 
    if(retVal == SOCKET_ERROR)
    {
        MessageBox(NULL, L"Unable to send()", L"ERROR", MB_OK);
        WSACleanup();
        return 1;
    }
 int q = 0;
 
	while(q<20)
	{
		q++;
		char szResponse[2048];
		retVal = recv(clientSock, szResponse, 2048, 0);
		int k = 0;
		if (szResponse!=NULL)
		for(int i = 0; i<3; i++)
			if(szResponse[i] == '+')
				k++;
		if (k == 3)
			break;
		szResponse[retVal]='\0';
	
		if(retVal == SOCKET_ERROR)
		{
			MessageBox(NULL, L"Unable to recv()", L"SOCKET ERROR", MB_OK);
			WSACleanup();
			return 1;
		}
				
		printf("\n%s\n",szResponse);
	}

	
    closesocket(clientSock);
    WSACleanup();
	system("pause");
    
    return 0;
}