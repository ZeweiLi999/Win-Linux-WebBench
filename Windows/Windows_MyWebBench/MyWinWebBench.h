#ifndef _MYWINWEBBENCH_H_
#define _MYWINWEBBENCH_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_CLIENTS 30000  // 定义最大客户端数量
int Bench(const char* url, int clients, int benchtime, int force, int http10, int proxyport,int method);
#endif // !1

