// FuckMutex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cstdio>  
#include <windows.h>  

#include "../Detours/detours.h"
#pragma comment(lib, "../Detours/detours.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,  
					   HINSTANCE hPrevInstance,  
					   LPTSTR    lpCmdLine,  
					   int       nCmdShow)  
{  
	STARTUPINFO si;     
	PROCESS_INFORMATION pi;  
	ZeroMemory(&si, sizeof(STARTUPINFO));     
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));     
	si.cb = sizeof(STARTUPINFO);     
	char* DirPath = new char[MAX_PATH];     
	char* DLLPath = new char[MAX_PATH];   
	GetCurrentDirectoryA(MAX_PATH, DirPath);     
	sprintf_s(DLLPath, MAX_PATH, "%s\\Inject.dll", DirPath); 
	if(lpCmdLine == NULL)
	{
		MessageBoxA(NULL,"Invalid args","Notics",MB_OK);
	}
	else	
	{
		DetourCreateProcessWithDllA(
		NULL,
		lpCmdLine, 
		NULL,
		NULL, 
		FALSE, 
		CREATE_DEFAULT_ERROR_MODE, 
		NULL, 
		NULL,
		&si, 
		&pi,
		DLLPath,
		NULL
		);     
	}
	delete [] DirPath;     
	delete [] DLLPath;

	return 0;
}  
