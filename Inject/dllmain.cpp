// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#undef UNICODE  
#include <cstdio> 
#include <time.h>
#include <windows.h>  
#include "../Detours/detours.h"
#pragma comment(lib, "../Detours/detours.lib")

__declspec(dllexport) void ExportFunc(void)
{
}

static HANDLE (WINAPI* OLD_CreateMutexA)(
			 __in_opt LPSECURITY_ATTRIBUTES lpMutexAttributes,
			 __in     BOOL bInitialOwner,
			 __in_opt LPCSTR lpName
			 ) = CreateMutexA;  

HANDLE WINAPI NEW_CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName)  
{
	if(lpName != NULL)
	{
		DWORD t1 = GetTickCount();
		DWORD t2 = GetTickCount();
		char buf[0x17];
		memset(buf,0,17);
		sprintf(buf,"%08X%08X",t1,t2);
		return OLD_CreateMutexA(lpMutexAttributes,bInitialOwner,buf);
	}
	else
	{
		return OLD_CreateMutexA(lpMutexAttributes,bInitialOwner,lpName);
	}  
}

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)  
{     
	switch(Reason)     
	{     
	case DLL_PROCESS_ATTACH:         
		{             
			DisableThreadLibraryCalls(hDLL);             
			DetourTransactionBegin();             
			DetourUpdateThread(GetCurrentThread());             
			DetourAttach(&(PVOID&)OLD_CreateMutexA, NEW_CreateMutexA);             
			DetourTransactionCommit();         
		}         
		break;     
	case DLL_PROCESS_DETACH:         
		DetourTransactionBegin();         
		DetourUpdateThread(GetCurrentThread());         
		DetourDetach(&(PVOID&)OLD_CreateMutexA, NEW_CreateMutexA);         
		DetourTransactionCommit();         
		break;     
	case DLL_THREAD_ATTACH:     
	case DLL_THREAD_DETACH:         
		break;     
	}     
	return TRUE;  
}  


