#ifndef _MYSOCKET_H_
#define _MYSOCKET_H_

#include <ws2tcpip.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

//确保生成环境链接到 Winsock 库文件Ws2_32.lib
#pragma comment(lib,"ws2_32.lib")

//前向声明，返回创建连接的客户端套接字
int MySocket(const char* host, int clientport);

#endif 

