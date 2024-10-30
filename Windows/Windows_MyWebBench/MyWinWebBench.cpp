#include "MyWinWebBench.h"
#include "MyWebBenchCore.h"


//��ѹ�����Ժ�����������������ӽ��̲��ȴ��������
int Bench(const char* url,int clients, int benchtime, int force,int http10, int proxyport,int method)
{
	PROCESS_INFORMATION* procinfo;  // ���ڴ洢ÿ���ӽ��̵Ľ�����Ϣ
	STARTUPINFO si;                 // �����ӽ���ʱʹ�õ�������Ϣ
	char commandLine[512];          // ���ڴ洢�ӽ��̵������в���

	// ���ݿͻ������������ڴ�
	procinfo = (PROCESS_INFORMATION*)malloc(clients * sizeof(PROCESS_INFORMATION));
	if (procinfo == NULL)
	{
		printf("�ڴ����ʧ��\n");
		return -1;
	}

	// ��ʼ��������Ϣ
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	// ѭ����������ӽ���
	for (int i = 0; i < clients; i++)
	{
		ZeroMemory(&procinfo[i], sizeof(PROCESS_INFORMATION));

		// ���������в���
		sprintf_s(commandLine, sizeof(commandLine), "BenchCore.exe -u %s -t %d -f %d -h %d -p %d -m %d", url, benchtime, force, http10, proxyport,method);

		// �����ӽ���
		if (!CreateProcess(
			NULL,               // Ӧ�ó�������
			commandLine,        // �����в���
			NULL,               // Ĭ�Ͻ��̰�ȫ����
			NULL,               // Ĭ���̰߳�ȫ����
			FALSE,              // ���̳о��
			0,                  // ��ʹ�����ⴴ����־
			NULL,               // ʹ�ø����̻�������
			NULL,               // ʹ�ø����̵�ǰ����Ŀ¼
			&si,                // ָ��������Ϣ��ָ��
			&procinfo[i]))      // ָ�������Ϣ��ָ��
		{
			printf("���̴���ʧ��\n");
			free(procinfo);
			return -1;
		}
	}
	// �ȴ������ӽ������
	for (int i = 0; i < clients; i++)
	{
		WaitForSingleObject(procinfo[i].hProcess, INFINITE);
		CloseHandle(procinfo[i].hProcess);
		CloseHandle(procinfo[i].hThread);
	}
	// �ͷ��ڴ�
	free(procinfo);
	return 0;
}

int main(int argc, char* argv[])
{
	// Ĭ�ϲ���
	char url[HOST_NAME_MAX];
	int benchtime = 30;     // ѹ������ʱ�䣬Ĭ��30��
	int clients = 1;        // �����ͻ���������Ĭ��1��
	int force = 0;          // �Ƿ�ǿ��ˢ�£�Ĭ�ϲ�ǿ��ˢ��
	int http10 = 1;         // HTTP �汾��Ĭ��ʹ��HTTP 1.0
	int proxyport = 80;     // ����˿ںţ�Ĭ��80
	int method = METHOD_GET; // Ĭ�ϲ���GET���ӷ�ʽ

	// ��������в���������
	if (argc < 2) {
		printf("\n�÷�������URL�Ϳ�����\n");
		printf("\n��������������������������������������������������������\n");
		printf("  -u <url>           ָ��Ҫ���Ե� URL				(���磺http://www.baidu.com/)\n");
		printf("  -t <ʱ��>          ָ������ʱ�� (��)				(Ĭ��: %d)\n", benchtime);
		printf("  -c <�ͻ�������>    ָ�������ͻ�������				(Ĭ��: %d)\n", clients);
		printf("  -f <ǿ�ƶ�ȡģʽ>  ǿ��ģʽ����������ȡ��Ӧ			(Ĭ�ϲ�����)\n");
		printf("  -h <HTTPЭ��>		ʹ�� HTTP 1.1				(Ĭ��: HTTP 1.0)\n");
		printf("  -p <�˿�>          ָ������˿ں�				(Ĭ��: %d)\n", proxyport);
		printf("  -m <����ʽ>      ָ������ʽ				(Ĭ��: GET)\n");
		return 1;
	}

	// ���������в���
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-u") == 0) {
			if (i + 1 < argc) {
				strncpy_s(url, sizeof(url), argv[++i], HOST_NAME_MAX - 1);
			}
			else {
				fprintf(stderr, "����: ȱ�� URL ������\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-t") == 0) {
			if (i + 1 < argc) {
				benchtime = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "����: ȱ��ʱ�������\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-c") == 0) {
			if (i + 1 < argc) {
				clients = atoi(argv[++i]);
				if (clients > MAX_CLIENTS) {
					printf("�ͻ������������������ %d\n", MAX_CLIENTS);
					return 1;
				}
			}
			else {
				fprintf(stderr, "����: ȱ�ٿͻ�������������\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-f") == 0) {
			if (i + 1 < argc) {
				force = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "����: ȱ��ǿ�Ʋ�����\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-h") == 0) {
			if (i + 1 < argc) {
				http10 = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "����: ȱ��httpЭ�������\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-p") == 0) {
			if (i + 1 < argc) {
				proxyport = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "����: ȱ�ٶ˿ڲ�����\n");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-m") == 0) {
			if (i + 1 < argc) {
				method = atoi(argv[++i]);
			}
			else {
				fprintf(stderr, "����: ȱ�����󷽷�������\n");
				return 1;
			}
		}
		else {
			printf("δ֪����: %s\n", argv[i]);
			return 1;
		}
	}

	
	char host1[HOST_NAME_MAX];
	char request1[REQUEST_SIZE];
	Build_Request(url, host1, request1, method, force, http10, proxyport);
	printf("������Ϣ��\n");
	printf("\nRequest:\n%s\n", request1);
	Check_Server(host1, proxyport);
	// ����ѹ������
	Bench(url, clients, benchtime, force,http10, proxyport,method);

	return 0;
}