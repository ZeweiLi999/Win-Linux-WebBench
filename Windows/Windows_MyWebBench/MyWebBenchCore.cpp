#include "MySocket.h"
#include "MyWebBenchCore.h"

//Build_Request��������url����������http����
void Build_Request(
	const char* url,
	char* host,                // ����host����ָ��
	char* request,             // ����request����ָ��
	int method,   // Ĭ�ϲ���GET���ӷ�ʽ
	int force,      // Ĭ�ϲ�����ǿ�Ƽ���
	int http10,            // Ĭ��ʹ��http10Э��
	int proxyport)        // Ĭ��Ϊ80�˿�
{
	char temp[10];  // ������ʱ�洢�˿ںŵ��ַ���
	int dot;         // ���ڱ�� URL �еĹؼ�λ��

	// ��ʼ����������
	memset(host, 0, HOST_NAME_MAX);
	memset(request, 0, REQUEST_SIZE);

	// ǿ��ˢ��ҳ����http�汾С��1.0ʱǿ��ʹ��http1.0
	if (force && http10 < 1)
		http10 = 1;

	// �������󷽷������������ʼ��
	switch (method)
	{
	default:
	case METHOD_GET: strcpy_s(request, REQUEST_SIZE, "GET"); break;
	case METHOD_HEAD: strcpy_s(request, REQUEST_SIZE, "HEAD"); break;
	case METHOD_OPTIONS: strcpy_s(request, REQUEST_SIZE, "OPTIONS"); break;
	case METHOD_TRACE: strcpy_s(request, REQUEST_SIZE, "TRACE"); break;
	}
	strcat_s(request, REQUEST_SIZE, " ");  // ��ӿո��Ա����� URL

	// ��� URL �Ƿ���Ч��������� "://"��
	if (NULL == strstr(url, "://"))
	{
		printf("\n%s: ����һ���Ϸ���URL.\n", url);
		exit(2);
	}

	// ��� URL �ĳ����Ƿ񳬹���������ֵ
	if (strlen(url) > HOST_NAME_MAX)
	{
		printf("\n%s: URL̫����.\n", url);
		exit(2);
	}

	// ���Э���Ƿ�Ϊ HTTP����֧�� HTTP Э��
	if (0 != _strnicmp("http://", url, 7))
	{
		printf("\nֻ֧��HTTPЭ��\n");
		exit(2);
	}

	// �ҵ� "://" ֮���λ�ã�������Э�鲿��
	dot = strstr(url, "://") - url + 3;

	// ��������������Ƿ��� '/' ��β����û���򱨴�
	if (strchr(url + dot, '/') == NULL) {
		printf("\nURLû���� '/'��β��\n");
		exit(2);
	}

	// ��������������ִ��� ':'��˵�������˶˿ں�
	if (strchr(url + dot, ':') != NULL && strchr(url + dot, ':') < strchr(url + dot, '/'))
	{
		// ��ȡ������
		strncpy_s(host, HOST_NAME_MAX, url + dot, strchr(url + dot, ':') - (url + dot));

		// ��� temp ���鲢�洢�˿ں��ַ���
		memset(temp, 0, sizeof(temp));
		strncpy_s(temp, sizeof(temp), strchr(url + dot, ':') + 1, strchr(url + dot, '/') - strchr(url + dot, ':') - 1);

		// ���ַ�����ʽ�Ķ˿ں�ת��Ϊ����
		proxyport = atoi(temp);

		// ���ת��ʧ�ܣ���Ĭ��Ϊ�˿� 80
		if (proxyport == 0) proxyport = 80;
	}
	else
	{
		// ���û�ж˿ںţ�ֱ����ȡ������
		strncpy_s(host, HOST_NAME_MAX, url + dot, strcspn(url + dot, "/"));
	}

	// ��·������׷�ӵ������ַ�����
	const char* path = url + dot + strcspn(url + dot, "/");
	if (strlen(request) + strlen(path) >= REQUEST_SIZE) {
		printf("���󹹽�ʧ�ܣ��������󻺳�����С��\n");
		exit(2);
	}
	strcat_s(request, REQUEST_SIZE, path);

	// ���� HTTP �汾׷��Э��汾
	if (http10 == 1)
		strcat_s(request, REQUEST_SIZE, " HTTP/1.0");
	else if (http10 == 2)
		strcat_s(request, REQUEST_SIZE, " HTTP/1.1");

	strcat_s(request, REQUEST_SIZE, "\r\n");  // ��ӻس��ͻ��б�ʾ�����н���

	// ���ʹ�� HTTP 1.0 �� 1.1����� User-Agent
	if (http10 > 0)
		strcat_s(request, REQUEST_SIZE, "User-Agent: MyWinWebBench " PROGRAM_VERSION "\r\n");

	// HTTP �汾���� 0.9����� Host �ֶ�
	if (http10 > 0)
	{
		strcat_s(request, REQUEST_SIZE, "Host: ");
		strcat_s(request, REQUEST_SIZE, host);
		strcat_s(request, REQUEST_SIZE, "\r\n");
	}

	// ���ʹ�� HTTP/1.1����� Connection �ֶ�
	if (http10 > 1)
		strcat_s(request, REQUEST_SIZE, "Connection: close\r\n");

	// HTTP ����ĩβ��Ҫһ������
	if (http10 > 0) strcat_s(request, REQUEST_SIZE, "\r\n");

	// ��ӡ������Ϣ
	/*printf("������Ϣ��\n");
	printf("\nRequest:\n%s\n", request);*/
}

//Check_Server���������ڶಢ�����ǰ�������״����
int Check_Server(char* host, int proxyport)
{
	int clientSocket = MySocket(host, proxyport);
	if (clientSocket < 0) {
		// ��������������������Ϣ
		printf("ѹ������ǰ���ӷ�����ʧ�ܣ�socket��������%d\n", clientSocket);
		return 1;
	}
	//printf("ѹ������ǰ���ӷ������ɹ���socket��������%d\n", clientSocket);
	// �������� socket�����緢��/��������

	// �ر� socket
	closesocket(clientSocket);
	WSACleanup(); // ���� Winsock
	return 0;
}

//�������̵�ѹ�����Ժ���
// �ӽ���ִ�еĺ��Ĺ���
void BenchCore(
	char* host,				//����host����ָ��
	char* request,			//����request����ָ��
	int benchtime, //Ĭ��Ϊ30��
	int force,  //Ĭ�ϲ�����
	int proxyport) //�˿ں�Ĭ��Ϊ80
{
	int rlen;      // HTTP����ĳ���
	char buf[REQUEST_SIZE];  // ���ڴ洢�ӷ��������յ�����
	int s, i;      // sΪsocket��������iΪ��ȡ�����ݳ���
	int localSpeed = 0;   // ��ǰ�ӽ��̵ĳɹ��������
	int localFailed = 0;  // ��ǰ�ӽ��̵�ʧ���������
	int localBytes = 0;   // ��ǰ�ӽ��̽��յ������ֽ���

	// ����HTTP����ĳ���
	rlen = strlen(request);

	// ��ȡ���Կ�ʼ��ʱ�䣬�����ж��Ƿ񵽴���Խ���ʱ��
	DWORD startTime = GetTickCount64();

	// ����ѭ��������ִ�У�ֱ���趨�Ĳ���ʱ�����
	while (GetTickCount64() - startTime < benchtime * 1000) {
		// ������Ŀ�������������
		s = MySocket(host, proxyport);
		if (s < 0) {  // �����������ʧ�ܣ���¼ʧ�ܴ�����������һ�γ���
			localFailed++;
			continue;
		}

		// ���͹����õ�HTTP����
		if (send(s, request, rlen, 0) != rlen) {
			// ������͵����ݳ��Ȳ��������󳤶ȣ���Ϊ����ʧ��
			localFailed++;
			closesocket(s);  // �ر�socket����
			continue;
		}

		// �������Ҫ������ȡ��������Ӧ���ݣ�����������ȡ����
		if (force == 0) {
			while (GetTickCount64() - startTime < benchtime * 1000) {
				// �ӷ�������ȡ����
				i = recv(s, buf, sizeof(buf), 0);
				if (i < 0) 
				{
					// �����ȡʧ�ܣ���¼ʧ�ܴ������ر�socket����
					localFailed++;
					closesocket(s);
					goto next_try;
				}
				else if (i == 0) 
				{
					// �����ȡ������ĩβ��0��ʾ���ӹرգ���������ȡ
					break;
				}
				else 
				{
					// �ɹ���ȡ���ݣ��ۼӽ��յ����ֽ���
					localBytes += i;
				}
			}
		}

		// �ر�socket���ӣ������ӳɹ�����ļ���
		closesocket(s);
		localSpeed++;

	next_try:
		continue;
	}

	// ���Խ���������������̨������д�빲���ڴ���ļ����ڽ��̼�ͨ��
	printf("�ӽ������: �ɹ�������: %d, ʧ��������: %d, �����ֽ���: %d\n", localSpeed, localFailed, localBytes);
	// ������Խ�������ظ������̻���д�빲���ڴ�����
	exit(0);  // ������ǰ�ӽ���
}

//int main(int argc, char* argv[])
//{
//	// Ĭ�ϲ���
//	char url[HOST_NAME_MAX];
//	int benchtime = 30;     // ѹ������ʱ�䣬Ĭ��30��
//	int force = 0;          // �Ƿ�ǿ��ˢ�£�Ĭ�ϲ�ǿ��ˢ��
//	int http10 = 1;         // HTTP �汾��Ĭ��ʹ��HTTP 1.0
//	int proxyport = 80;     // ����˿ںţ�Ĭ��80
//	int method = METHOD_GET; // Ĭ�ϲ���GET���ӷ�ʽ
//
//	// ��������в���������
//	if (argc < 2) {
//		printf("\n�÷�������URL�Ϳ�����\n");
//		printf("\n��������������������������������������������������������\n");
//		printf("  -u <url>           ָ��Ҫ���Ե� URL				(���磺http://www.baidu.com/)\n");
//		printf("  -t <ʱ��>          ָ������ʱ�� (��)				(Ĭ��: %d)\n", benchtime);
//		printf("  -f <ǿ�ƶ�ȡģʽ>  ǿ��ģʽ����������ȡ��Ӧ			(Ĭ�ϲ�����)\n");
//		printf("  -h <HTTPЭ��>		ʹ�� HTTP 1.1				(Ĭ��: HTTP 1.0)\n");
//		printf("  -p <�˿�>          ָ������˿ں�				(Ĭ��: %d)\n", proxyport);
//		printf("  -m <����ʽ>      ָ������ʽ				(Ĭ��: GET)\n");
//		return 1;
//	}
//
//	// ���������в���
//	for (int i = 1; i < argc; i++) {
//		if (strcmp(argv[i], "-u") == 0) {
//			if (i + 1 < argc) {
//				strncpy_s(url, sizeof(url), argv[++i], HOST_NAME_MAX - 1);
//			}
//			else {
//				fprintf(stderr, "����: ȱ�� URL ������\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-t") == 0) {
//			if (i + 1 < argc) {
//				benchtime = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "����: ȱ��ʱ�������\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-f") == 0) {
//			if (i + 1 < argc) {
//				force = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "����: ȱ��ǿ�Ʋ�����\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-h") == 0) {
//			if (i + 1 < argc) {
//				http10 = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "����: ȱ��httpЭ�������\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-p") == 0) {
//			if (i + 1 < argc) {
//				proxyport = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "����: ȱ�ٶ˿ڲ�����\n");
//				return 1;
//			}
//		}
//		else if (strcmp(argv[i], "-m") == 0) {
//			if (i + 1 < argc) {
//				method = atoi(argv[++i]);
//			}
//			else {
//				fprintf(stderr, "����: ȱ�����󷽷�������\n");
//				return 1;
//			}
//		}
//		else {
//			printf("δ֪����: %s\n", argv[i]);
//			return 1;
//		}
//	}
//
//	// ��ʼ�� host �� request ����
//	char host[HOST_NAME_MAX];
//	char request[REQUEST_SIZE];
//
//	// ���� HTTP ����
//	Build_Request(url, host, request, method, force, http10, proxyport);
//
//	// ����ѹ������
//	BenchCore(host, request, benchtime, force, proxyport);
//	
//	return 0;
//}


