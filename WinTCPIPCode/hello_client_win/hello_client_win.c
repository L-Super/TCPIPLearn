//
// Created by Listening on 2022/2/2.
//
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hScket;
	SOCKADDR_IN servAddr;
	char message[30];
	if(argc != 3)
	{
		printf("Usage: %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2, 2),&wsaData) != 0)
		printf("WSAStartup() error");

	hScket = socket(PF_INET,SOCK_STREAM,0);
	if(hScket == INVALID_SOCKET)
		printf("socket() error");
	memset(&servAddr,0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	// inet_addr() is deprecated,use inet_pton()
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(argv[2]);

	if(connect(hScket,(SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		printf("connect() error\n");

	int strLen = recv(hScket,message, sizeof(message) - 1, 0);
	if(strLen == -1)
		printf("read() error\n");

	printf("Message from server: %s \n", message);

	closesocket(hScket);
	WSACleanup();
	return 0;
}
