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

//��������http����
void Build_Request(
	const char* url,
	char* host,				//����host����ָ��
	char* request,			//����request����ָ��
	int method,//Ĭ�ϲ���GET���ӷ�ʽ
	int force,	//Ĭ�ϲ�����ǿ�Ƽ���
	int http10,			//Ĭ��ʹ��http10Э��
	int proxyport);		//Ĭ��Ϊ80�˿�
//����http���Եĺ���
void BenchCore(
	char* host,				//����host����ָ��
	char* request,			//����request����ָ��
	int benchtime, //����ʱ�䣬Ĭ��Ϊ30��
	int force,
	int proxyport);//���Զ˿�

//ѹ������ǰ�����Ƿ�����ͨ������
int Check_Server(char* host, int proxyport);
#endif 
