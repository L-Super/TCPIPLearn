//
// Created by Listening on 2022/2/2.
//
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
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
	// 初始化，设置程序用到的Winsock版本，并初始化相应版本的库
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		printf("WSAStartup() error!");
	}
	// 返回套接字句柄
	hServerSock = socket(PF_INET,SOCK_STREAM,0);
	if(hServerSock == INVALID_SOCKET)
	{
		printf("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if(bind(hServerSock,(SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
		printf("bind() error");
	if(listen(hServerSock, 5) == SOCKET_ERROR)
		printf("listen() error");

	szClientAddr = sizeof(clientAddr);
	hClientSock = accept(hServerSock,(SOCKADDR*)&clientAddr,&szClientAddr);
	if(hClientSock == INVALID_SOCKET)
		printf("accept() error");
	send(hClientSock,message,sizeof(message),0);

	// 关闭套接字
	closesocket(hClientSock);
	closesocket(hServerSock);
	// 注销库
	WSACleanup();
	return 0;
}

