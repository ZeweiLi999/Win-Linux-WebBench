#include "MySocket.h"

int MySocket(const char* host, int clientport)
{
	WSADATA wsdata; //��Windows socket��ǰ����ʼ��WinSock
	SOCKET sock;	//���ڴ洢socket������
	unsigned long inaddr; //���ڴ洢ת�����IP��ַ
	struct sockaddr_in server_addr; // �洢��������ַ�Ͷ˿�
	struct addrinfo hints, * res; // ���ڴ洢������ĵ�ַ��Ϣ

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&hints, 0, sizeof(hints));

	//��ʼ���׽��ֽṹ
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0)
	{
		printf("��ʼ��Windows Socket ʧ��");
	}

	// ���� hints �������� IPv4 TCP
	//hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = 0;

	// ���� getaddrinfo ����������
	if (getaddrinfo(host,NULL, &hints, &res) != 0) {
		printf("getaddrinfo ��������ʧ��\n");
		WSACleanup();
		return -1;
	}

	memcpy(&server_addr, res->ai_addr, sizeof(server_addr));
	server_addr.sin_port = htons(clientport); // ���ö˿ں�

	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock == INVALID_SOCKET)
	{
		printf("TCP Socket����ʧ��");
		freeaddrinfo(res); // �ͷŵ�ַ��Ϣ
		closesocket(sock);
		WSACleanup();
		return -1; 
	}

	if (connect(sock, (struct sockaddr*)&server_addr , sizeof(server_addr))<0)
	{
		printf("����������ʧ��\n");
		freeaddrinfo(res);
		closesocket(sock);
		WSACleanup();
		return -1;
	}
	freeaddrinfo(res); // �ͷŵ�ַ��Ϣ
	return sock; // ���ؿͻ��� socket ������
}