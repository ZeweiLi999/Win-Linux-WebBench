#include "MySocket.h"

int MySocket(const char* host, int clientport)
{
	WSADATA wsdata; //用Windows socket库前，初始化WinSock
	SOCKET sock;	//用于存储socket描述符
	unsigned long inaddr; //用于存储转换后的IP地址
	struct sockaddr_in server_addr; // 存储服务器地址和端口
	struct addrinfo hints, * res; // 用于存储解析后的地址信息

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&hints, 0, sizeof(hints));

	//初始化套接字结构
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0)
	{
		printf("初始化Windows Socket 失败");
	}

	// 设置 hints 过滤条件 IPv4 TCP
	//hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = 0;

	// 调用 getaddrinfo 解析主机名
	if (getaddrinfo(host,NULL, &hints, &res) != 0) {
		printf("getaddrinfo 函数调用失败\n");
		WSACleanup();
		return -1;
	}

	memcpy(&server_addr, res->ai_addr, sizeof(server_addr));
	server_addr.sin_port = htons(clientport); // 设置端口号

	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock == INVALID_SOCKET)
	{
		printf("TCP Socket连接失败");
		freeaddrinfo(res); // 释放地址信息
		closesocket(sock);
		WSACleanup();
		return -1; 
	}

	if (connect(sock, (struct sockaddr*)&server_addr , sizeof(server_addr))<0)
	{
		printf("服务器连接失败\n");
		freeaddrinfo(res);
		closesocket(sock);
		WSACleanup();
		return -1;
	}
	freeaddrinfo(res); // 释放地址信息
	return sock; // 返回客户端 socket 描述符
}