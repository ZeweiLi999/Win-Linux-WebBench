#ifndef _MYSOCKET_H_
#define _MYSOCKET_H_

#include <ws2tcpip.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

//ȷ�����ɻ������ӵ� Winsock ���ļ�Ws2_32.lib
#pragma comment(lib,"ws2_32.lib")

//ǰ�����������ش������ӵĿͻ����׽���
int MySocket(const char* host, int clientport);

#endif 

