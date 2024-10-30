#ifndef _MYWEBBENCHCORE_H_
#define _MYWEBBENCHCORE_H_
#include <string.h>
#include <process.h> 
#include <time.h>
#include <signal.h>
#define REQUEST_SIZE 2048
#define HOST_NAME_MAX 256
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define PROGRAM_VERSION "1.0"

//用来构建http请求
void Build_Request(
	const char* url,
	char* host,				//接收host数组指针
	char* request,			//接收request数组指针
	int method,//默认采用GET连接方式
	int force,	//默认不开启强制加载
	int http10,			//默认使用http10协议
	int proxyport);		//默认为80端口
//单次http测试的函数
void BenchCore(
	char* host,				//接收host数组指针
	char* request,			//接收request数组指针
	int benchtime, //测试时间，默认为30秒
	int force,
	int proxyport);//测试端口

//压力测试前测试是否能连通服务器
int Check_Server(char* host, int proxyport);
#endif 
