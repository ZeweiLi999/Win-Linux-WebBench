#include "MySocket.h"
#include "MyWebBenchCore.h"

//Build_Request函数是用url参数来构建http请求
void Build_Request(
	const char* url,
	char* host,                // 接收host数组指针
	char* request,             // 接收request数组指针
	int method,   // 默认采用GET连接方式
	int force,      // 默认不开启强制加载
	int http10,            // 默认使用http10协议
	int proxyport)        // 默认为80端口
{
	char temp[10];  // 用于临时存储端口号的字符串
	int dot;         // 用于标记 URL 中的关键位置

	// 初始化两个数组
	memset(host, 0, HOST_NAME_MAX);
	memset(request, 0, REQUEST_SIZE);

	// 强制刷新页面且http版本小于1.0时强制使用http1.0
	if (force && http10 < 1)
		http10 = 1;

	// 根据请求方法设置请求的起始行
	switch (method)
	{
	default:
	case METHOD_GET: strcpy_s(request, REQUEST_SIZE, "GET"); break;
	case METHOD_HEAD: strcpy_s(request, REQUEST_SIZE, "HEAD"); break;
	case METHOD_OPTIONS: strcpy_s(request, REQUEST_SIZE, "OPTIONS"); break;
	case METHOD_TRACE: strcpy_s(request, REQUEST_SIZE, "TRACE"); break;
	}
	strcat_s(request, REQUEST_SIZE, " ");  // 添加空格以便连接 URL

	// 检查 URL 是否有效（必须包含 "://"）
	if (NULL == strstr(url, "://"))
	{
		printf("\n%s: 不是一个合法的URL.\n", url);
		exit(2);
	}

	// 检查 URL 的长度是否超过允许的最大值
	if (strlen(url) > HOST_NAME_MAX)
	{
		printf("\n%s: URL太长了.\n", url);
		exit(2);
	}

	// 检查协议是否为 HTTP，仅支持 HTTP 协议
	if (0 != _strnicmp("http://", url, 7))
	{
		printf("\n只支持HTTP协议\n");
		exit(2);
	}

	// 找到 "://" 之后的位置，即跳过协议部分
	dot = strstr(url, "://") - url + 3;

	// 检查主机名部分是否以 '/' 结尾，若没有则报错
	if (strchr(url + dot, '/') == NULL) {
		printf("\nURL没有以 '/'结尾！\n");
		exit(2);
	}

	// 如果在主机名部分存在 ':'，说明包含了端口号
	if (strchr(url + dot, ':') != NULL && strchr(url + dot, ':') < strchr(url + dot, '/'))
	{
		// 提取主机名
		strncpy_s(host, HOST_NAME_MAX, url + dot, strchr(url + dot, ':') - (url + dot));

		// 清空 temp 数组并存储端口号字符串
		memset(temp, 0, sizeof(temp));
		strncpy_s(temp, sizeof(temp), strchr(url + dot, ':') + 1, strchr(url + dot, '/') - strchr(url + dot, ':') - 1);

		// 将字符串形式的端口号转换为整数
		proxyport = atoi(temp);

		// 如果转换失败，则默认为端口 80
		if (proxyport == 0) proxyport = 80;
	}
	else
	{
		// 如果没有端口号，直接提取主机名
		strncpy_s(host, HOST_NAME_MAX, url + dot, strcspn(url + dot, "/"));
	}

	// 将路径部分追加到请求字符串中
	const char* path = url + dot + strcspn(url + dot, "/");
	if (strlen(request) + strlen(path) >= REQUEST_SIZE) {
		printf("请求构建失败，超出请求缓冲区大小。\n");
		exit(2);
	}
	strcat_s(request, REQUEST_SIZE, path);

	// 根据 HTTP 版本追加协议版本
	if (http10 == 1)
		strcat_s(request, REQUEST_SIZE, " HTTP/1.0");
	else if (http10 == 2)
		strcat_s(request, REQUEST_SIZE, " HTTP/1.1");

	strcat_s(request, REQUEST_SIZE, "\r\n");  // 添加回车和换行表示请求行结束

	// 如果使用 HTTP 1.0 或 1.1，添加 User-Agent
	if (http10 > 0)
		strcat_s(request, REQUEST_SIZE, "User-Agent: MyWinWebBench " PROGRAM_VERSION "\r\n");

	// HTTP 版本大于 0.9，添加 Host 字段
	if (http10 > 0)
	{
		strcat_s(request, REQUEST_SIZE, "Host: ");
		strcat_s(request, REQUEST_SIZE, host);
		strcat_s(request, REQUEST_SIZE, "\r\n");
	}

	// 如果使用 HTTP/1.1，添加 Connection 字段
	if (http10 > 1)
		strcat_s(request, REQUEST_SIZE, "Connection: close\r\n");

	// HTTP 请求末尾需要一个空行
	if (http10 > 0) strcat_s(request, REQUEST_SIZE, "\r\n");

	// 打印请求信息
	/*printf("请求信息：\n");
	printf("\nRequest:\n%s\n", request);*/
}

//Check_Server函数是用在多并发检测前检查连接状况的
int Check_Server(char* host, int proxyport)
{
	int clientSocket = MySocket(host, proxyport);
	if (clientSocket < 0) {
		// 处理错误，例如输出错误信息
		printf("压力测试前连接服务器失败！socket描述符：%d\n", clientSocket);
		return 1;
	}
	//printf("压力测试前连接服务器成功！socket描述符：%d\n", clientSocket);
	// 继续处理 socket，例如发送/接收数据

	// 关闭 socket
	closesocket(clientSocket);
	WSACleanup(); // 清理 Winsock
	return 0;
}

//单个进程的压力测试函数
// 子进程执行的核心功能
void BenchCore(
	char* host,				//接收host数组指针
	char* request,			//接收request数组指针
	int benchtime, //默认为30秒
	int force,  //默认不重启
	int proxyport) //端口号默认为80
{
	int rlen;      // HTTP请求的长度
	char buf[REQUEST_SIZE];  // 用于存储从服务器接收的数据
	int s, i;      // s为socket描述符，i为读取的数据长度
	int localSpeed = 0;   // 当前子进程的成功请求计数
	int localFailed = 0;  // 当前子进程的失败请求计数
	int localBytes = 0;   // 当前子进程接收到的总字节数

	// 计算HTTP请求的长度
	rlen = strlen(request);

	// 获取测试开始的时间，用于判断是否到达测试结束时间
	DWORD startTime = GetTickCount64();

	// 测试循环，持续执行，直到设定的测试时间结束
	while (GetTickCount64() - startTime < benchtime * 1000) {
		// 创建到目标服务器的连接
		s = MySocket(host, proxyport);
		if (s < 0) {  // 如果创建连接失败，记录失败次数并继续下一次尝试
			localFailed++;
			continue;
		}

		// 发送构建好的HTTP请求
		if (send(s, request, rlen, 0) != rlen) {
			// 如果发送的数据长度不等于请求长度，视为发送失败
			localFailed++;
			closesocket(s);  // 关闭socket连接
			continue;
		}

		// 如果不需要完整读取服务器响应数据，可以跳过读取步骤
		if (force == 0) {
			while (GetTickCount64() - startTime < benchtime * 1000) {
				// 从服务器读取数据
				i = recv(s, buf, sizeof(buf), 0);
				if (i < 0) 
				{
					// 如果读取失败，记录失败次数并关闭socket连接
					localFailed++;
					closesocket(s);
					goto next_try;
				}
				else if (i == 0) 
				{
					// 如果读取到数据末尾（0表示连接关闭），结束读取
					break;
				}
				else 
				{
					// 成功读取数据，累加接收到的字节数
					localBytes += i;
				}
			}
		}

		// 关闭socket连接，并增加成功请求的计数
		closesocket(s);
		localSpeed++;

	next_try:
		continue;
	}

	// 测试结果可以输出到控制台，或者写入共享内存或文件用于进程间通信
	printf("子进程完成: 成功请求数: %d, 失败请求数: %d, 传输字节数: %d\n", localSpeed, localFailed, localBytes);
	// 这里可以将结果返回给父进程或者写入共享内存区域
	exit(0);  // 结束当前子进程
}

//int main(int argc, char* argv[])
//{
//	// 默认参数
//	char url[HOST_NAME_MAX];
//	int benchtime = 30;     // 压力测试时间，默认30秒
//	int force = 0;          // 是否强制刷新，默认不强制刷新
//	int http10 = 1;         // HTTP 版本，默认使用HTTP 1.0
//	int proxyport = 80;     // 代理端口号，默认80
//	int method = METHOD_GET; // 默认采用GET连接方式
//
//	// 检查命令行参数的数量
//	if (argc < 2) {
//		printf("\n用法：填入URL就可运行\n");
//		printf("\n————————————————————————————\n");
//		printf("  -u <url>           指定要测试的 URL				(例如：http://www.baidu.com/)\n");
//		printf("  -t <时间>          指定测试时间 (秒)				(默认: %d)\n", benchtime);
//		printf("  -f <强制读取模式>  强制模式，不完整读取响应			(默认不开启)\n");
//		printf("  -h <HTTP协议>		使用 HTTP 1.1				(默认: HTTP 1.0)\n");
//		printf("  -p <端口>          指定代理端口号				(默认: %d)\n", proxyport);
//		printf("  -m <请求方式>      指定请求方式				(默认: GET)\n");
//		return 1;
//	}
//
//	// 解析命令行参数
//	for (int i = 1; i < argc; i++) {
//		if (strcmp(argv[i], "-u") == 0) {
//			if (i + 1 < argc) {
//				strncpy_s(url, sizeof(url), argv[++i], HOST_NAME_MAX - 1);
//			}
//			else {
//				fprintf(stderr, "错误: 缺少 URL 参数。\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-t") == 0) {
//			if (i + 1 < argc) {
//				benchtime = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "错误: 缺少时间参数。\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-f") == 0) {
//			if (i + 1 < argc) {
//				force = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "错误: 缺少强制参数。\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-h") == 0) {
//			if (i + 1 < argc) {
//				http10 = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "错误: 缺少http协议参数。\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-p") == 0) {
//			if (i + 1 < argc) {
//				proxyport = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "错误: 缺少端口参数。\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-m") == 0) {
//			if (i + 1 < argc) {
//				method = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "错误: 缺少请求方法参数。\n");
//				return 1;
//			}
//		}
//		else {
//			printf("未知参数: %s\n", argv[i]);
//			return 1;
//		}
//	}
//
//	// 初始化 host 和 request 数组
//	char host[HOST_NAME_MAX];
//	char request[REQUEST_SIZE];
//
//	// 构建 HTTP 请求
//	Build_Request(url, host, request, method, force, http10, proxyport);
//
//	// 启动压力测试
//	BenchCore(host, request, benchtime, force, proxyport);
//	
//	return 0;
//}


