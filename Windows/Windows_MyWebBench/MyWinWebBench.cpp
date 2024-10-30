#include "MyWinWebBench.h"
#include "MyWebBenchCore.h"


//主压力测试函数，用于启动多个子进程并等待他们完成
int Bench(const char* url,int clients, int benchtime, int force,int http10, int proxyport,int method)
{
	PROCESS_INFORMATION* procinfo;  // 用于存储每个子进程的进程信息
	STARTUPINFO si;                 // 启动子进程时使用的启动信息
	char commandLine[512];          // 用于存储子进程的命令行参数

	// 根据客户端数量分配内存
	procinfo = (PROCESS_INFORMATION*)malloc(clients * sizeof(PROCESS_INFORMATION));
	if (procinfo == NULL)
	{
		printf("内存分配失败\n");
		return -1;
	}

	// 初始化启动信息
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	// 循环创建多个子进程
	for (int i = 0; i < clients; i++)
	{
		ZeroMemory(&procinfo[i], sizeof(PROCESS_INFORMATION));

		// 构建命令行参数
		sprintf_s(commandLine, sizeof(commandLine), "BenchCore.exe -u %s -t %d -f %d -h %d -p %d -m %d", url, benchtime, force, http10, proxyport,method);

		// 创建子进程
		if (!CreateProcess(
			NULL,               // 应用程序名称
			commandLine,        // 命令行参数
			NULL,               // 默认进程安全属性
			NULL,               // 默认线程安全属性
			FALSE,              // 不继承句柄
			0,                  // 不使用特殊创建标志
			NULL,               // 使用父进程环境变量
			NULL,               // 使用父进程当前工作目录
			&si,                // 指向启动信息的指针
			&procinfo[i]))      // 指向进程信息的指针
		{
			printf("进程创建失败\n");
			free(procinfo);
			return -1;
		}
	}
	// 等待所有子进程完成
	for (int i = 0; i < clients; i++)
	{
		WaitForSingleObject(procinfo[i].hProcess, INFINITE);
		CloseHandle(procinfo[i].hProcess);
		CloseHandle(procinfo[i].hThread);
	}
	// 释放内存
	free(procinfo);
	return 0;
}

int main(int argc, char* argv[])
{
	// 默认参数
	char url[HOST_NAME_MAX];
	int benchtime = 30;     // 压力测试时间，默认30秒
	int clients = 1;        // 并发客户端数量，默认1个
	int force = 0;          // 是否强制刷新，默认不强制刷新
	int http10 = 1;         // HTTP 版本，默认使用HTTP 1.0
	int proxyport = 80;     // 代理端口号，默认80
	int method = METHOD_GET; // 默认采用GET连接方式

	// 检查命令行参数的数量
	if (argc < 2) {
		printf("\n用法：填入URL就可运行\n");
		printf("\n————————————————————————————\n");
		printf("  -u <url>           指定要测试的 URL				(例如：http://www.baidu.com/)\n");
		printf("  -t <时间>          指定测试时间 (秒)				(默认: %d)\n", benchtime);
		printf("  -c <客户端数量>    指定并发客户端数量				(默认: %d)\n", clients);
		printf("  -f <强制读取模式>  强制模式，不完整读取响应			(默认不开启)\n");
		printf("  -h <HTTP协议>		使用 HTTP 1.1				(默认: HTTP 1.0)\n");
		printf("  -p <端口>          指定代理端口号				(默认: %d)\n", proxyport);
		printf("  -m <请求方式>      指定请求方式				(默认: GET)\n");
		return 1;
	}

	// 解析命令行参数
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-u") == 0) {
			if (i + 1 < argc) {
				strncpy_s(url, sizeof(url), argv[++i], HOST_NAME_MAX - 1);
			}
			else {
				fprintf(stderr, "错误: 缺少 URL 参数。\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-t") == 0) {
			if (i + 1 < argc) {
				benchtime = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "错误: 缺少时间参数。\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-c") == 0) {
			if (i + 1 < argc) {
				clients = atoi(argv[++i]);
				if (clients > MAX_CLIENTS) {
					printf("客户端数量超过最大限制 %d\n", MAX_CLIENTS);
					return 1;
				}
			}
			else {
				fprintf(stderr, "错误: 缺少客户端数量参数。\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-f") == 0) {
			if (i + 1 < argc) {
				force = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "错误: 缺少强制参数。\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-h") == 0) {
			if (i + 1 < argc) {
				http10 = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "错误: 缺少http协议参数。\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-p") == 0) {
			if (i + 1 < argc) {
				proxyport = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "错误: 缺少端口参数。\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-m") == 0) {
			if (i + 1 < argc) {
				method = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "错误: 缺少请求方法参数。\n");
				return 1;
			}
		}
		else {
			printf("未知参数: %s\n", argv[i]);
			return 1;
		}
	}

	
	char host1[HOST_NAME_MAX];
	char request1[REQUEST_SIZE];
	Build_Request(url, host1, request1, method, force, http10, proxyport);
	printf("请求信息：\n");
	printf("\nRequest:\n%s\n", request1);
	Check_Server(host1, proxyport);
	// 启动压力测试
	Bench(url, clients, benchtime, force,http10, proxyport,method);

	return 0;
}