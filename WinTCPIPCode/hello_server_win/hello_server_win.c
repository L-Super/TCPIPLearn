//
// Created by Listening on 2022/2/2.
//
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

int main()
{
	WSADATA wsaData;
	SOCKET hServerSock,hClientSock;
	SOCKADDR_IN servAddr,clientAddr;
	int szClientAddr;
	char message[] = "hello world!";
	if(argc != 2)
	{
		printf("Usage: %s <port>\n",argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		printf("WSAStartup() error!");
	}
	hServerSock = socket(PF_INET,SOCK_STREAM,0);

	return 0;
}

