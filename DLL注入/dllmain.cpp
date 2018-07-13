/* Replace "dll.h" with the name of your header */
#include "dll.h"
#include <windows.h>

DllClass::DllClass()
{

}

DllClass::~DllClass()
{

}

void DllClass::HelloWorld()
{
	MessageBox(0, "Hello World from DLL!\n","Hi",MB_ICONINFORMATION);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{   
	char * szProcessId = (char *)malloc(10 * sizeof(char)); 
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			_itoa(GetCurrentProcessId(), szProcessId, 10);
			MessageBox(NULL, szProcessId, "RemoteDLL",MB_ICONINFORMATION);
			break;
		}
		default:
		return TRUE; 
	}
	
	/* Return TRUE on success, FALSE on failure */
}
