#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WS2tcpip.h>
#include<string.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
#define BUF_SIZE 100
int main() {
	char name1[100] = { 0 }, name2[100] = { 0 };
	printf("请输入你的姓名：");
	gets_s(name1);
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
		printf("创建失败\n");
	else
		printf("创建成功\n");
	//绑定套接字
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;  //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr("10.133.11.132");  //具体的IP地址
	sockAddr.sin_port = htons(1234);  //端口
	int ok=bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	if (ok == SOCKET_ERROR)
		printf("绑定失败\n");
	else
		printf("绑定成功\n");
	//进入监听状态
	int li=listen(servSock, 20);
	if (li == SOCKET_ERROR)
		printf("监听失败\n");
	else
		printf("监听成功\n");
	//接收客户端请求
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	char buffer[BUF_SIZE] = { 0 };  //缓冲区
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	recv(clntSock, buffer, BUF_SIZE, 0);
	strcpy_s(name2, buffer);
	strcpy_s(buffer, name1);
	send(clntSock, buffer, BUF_SIZE, 0);
	closesocket(clntSock);  //关闭套接字
	memset(buffer, 0, BUF_SIZE);  //重置缓冲区
	while (1) {
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //接收客户端发来的数据
		printf("\t\t\t\t%s:\n\t\t%s", name2, buffer);
		printf("\n%s:\n\t", name1);
		gets_s(buffer);
		send(clntSock, buffer, BUF_SIZE, 0);  //将数据原样返回
		closesocket(clntSock);  //关闭套接字
		memset(buffer, 0, BUF_SIZE);  //重置缓冲区
	}
	//关闭套接字
	closesocket(servSock);
	//终止 DLL 的使用
	WSACleanup();
	return 0;
}