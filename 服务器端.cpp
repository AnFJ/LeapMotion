#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WS2tcpip.h>
#include<string.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll
#define BUF_SIZE 100
int main() {
	char name1[100] = { 0 }, name2[100] = { 0 };
	printf("���������������");
	gets_s(name1);
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�����׽���
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
		printf("����ʧ��\n");
	else
		printf("�����ɹ�\n");
	//���׽���
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr("10.133.11.132");  //�����IP��ַ
	sockAddr.sin_port = htons(1234);  //�˿�
	int ok=bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	if (ok == SOCKET_ERROR)
		printf("��ʧ��\n");
	else
		printf("�󶨳ɹ�\n");
	//�������״̬
	int li=listen(servSock, 20);
	if (li == SOCKET_ERROR)
		printf("����ʧ��\n");
	else
		printf("�����ɹ�\n");
	//���տͻ�������
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	char buffer[BUF_SIZE] = { 0 };  //������
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	recv(clntSock, buffer, BUF_SIZE, 0);
	strcpy_s(name2, buffer);
	strcpy_s(buffer, name1);
	send(clntSock, buffer, BUF_SIZE, 0);
	closesocket(clntSock);  //�ر��׽���
	memset(buffer, 0, BUF_SIZE);  //���û�����
	while (1) {
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //���տͻ��˷���������
		printf("\t\t\t\t%s:\n\t\t%s", name2, buffer);
		printf("\n%s:\n\t", name1);
		gets_s(buffer);
		send(clntSock, buffer, BUF_SIZE, 0);  //������ԭ������
		closesocket(clntSock);  //�ر��׽���
		memset(buffer, 0, BUF_SIZE);  //���û�����
	}
	//�ر��׽���
	closesocket(servSock);
	//��ֹ DLL ��ʹ��
	WSACleanup();
	return 0;
}