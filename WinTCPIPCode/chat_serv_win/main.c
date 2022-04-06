#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <winsock2.h>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUF_SIZE 100
#define MAX_CLNT 256

int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

void ErrorHanding(char* msg)
{
	fputs(msg, stderr);
	fputc('\n',stderr);
	exit(1);
}

//send to all
void SendMsg(char* msg, int len)
{
	WaitForSingleObject(hMutex,INFINITE);
	for (int i = 0; i < clntCnt; ++i)
	{
		send(clntSocks[i], msg, len, 0);
		printf("send msg :%s", msg);
	}
	ReleaseMutex(hMutex);
}

unsigned WINAPI HandleClnt(void* arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	int strLen = 0, i;
	char msg[BUF_SIZE];

	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) != 0)
	{
		SendMsg(msg, strLen);
	}

	WaitForSingleObject(hMutex,INFINITE);
	for (int i = 0; i < clntCnt; ++i)//remove disconnected client
	{
		if(hClntSock == clntSocks[i])
		{
			while (i++ < clntCnt-1)
				clntSocks[i] = clntSocks[i+1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	closesocket(hClntSock);
	return 0;
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	int clntAddrSize;
	HANDLE hThread;
	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		ErrorHanding("WSAStartup() error!");

	hMutex = CreateMutex(NULL, FALSE, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
//	char* addr = "81.70.249.165";
//	unsigned long adr = inet_addr(addr);
//	if(adr == INADDR_NONE)
//		printf("error conver\n");
//	else
//		printf("Conversion %#lx\n", adr);
//	servAddr.sin_addr.s_addr = htonl(adr);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHanding("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHanding("listen() error");

	while (1)
	{
		clntAddrSize = sizeof(clntAddr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAddrSize);

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClntSock;
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);
		printf("connectd client IP: %s\n", inet_ntoa(clntAddr.sin_addr));
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}
