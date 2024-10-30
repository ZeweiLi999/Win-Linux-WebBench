#include "MySocket.h"


//int main() {
//	int clientSocket = MySocket("www.baidu.com", 80);
//	if (clientSocket < 0) {
//		// 处理错误，例如输出错误信息
//		return 1;
//	}
//	printf("成功访问！%d", clientSocket);
//	// 继续处理 socket，例如发送/接收数据
//
//	// 关闭 socket
//	closesocket(clientSocket);
//	WSACleanup(); // 清理 Winsock
//	return 0;
//}