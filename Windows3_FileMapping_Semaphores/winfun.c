#include "winfun.h"

const char* semName = "SampleSemaphore";
const char* memName = "SampleMemory";

HANDLE init(char* source, char* arg)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(
		source,						
		arg,						
		NULL,						
		NULL,						
		FALSE,						
		CREATE_NEW_CONSOLE,			
		NULL,						
		NULL,						
		&si,
		&pi
	))
	{
		printf("Last CreateProcess error: %d", GetLastError());
		getch();
		exit(EXIT_FAILURE);
	}
	return pi.hProcess; 
}

HANDLE initSemaphore(char* str)
{
	SECURITY_ATTRIBUTES attr;
	HANDLE handle;

	attr.bInheritHandle = TRUE;			// inherits by child process
	attr.nLength = sizeof(attr);
	attr.lpSecurityDescriptor = NULL;

	handle = CreateSemaphore(
		&attr,		// Semaphore Security Attributes
		1,			// InitialCount
		1,			// MaximumCount
		semName		// Name
	);

	if (handle == NULL) 
	{
		printf("Last CreateSemaphore error: %d", GetLastError());
		getch();
		exit(EXIT_FAILURE);
	}
	return handle;
}


HANDLE initSharedMemory(char *str)
{
	SECURITY_ATTRIBUTES attr;
	HANDLE handle;

	attr.bInheritHandle = TRUE;
	attr.nLength = sizeof(attr);
	attr.lpSecurityDescriptor = NULL;
	handle = CreateFileMapping(
		INVALID_HANDLE_VALUE,		// creates projected file
		&attr,						// 
		PAGE_READWRITE,				// Allows views to be mapped for read-only, copy-on-write, or read/write access.
		0,							// high byte of size
		100,						// low byte of size
		memName						//
	);
	
	if (handle == NULL) 
	{
		printf("Last CreateFileMapping error: %d", GetLastError());
		getch();
		exit(EXIT_FAILURE);
	}
	return handle;
}

void* getSharedMemory(HANDLE id)
{
	return MapViewOfFile(
		id,
		FILE_MAP_ALL_ACCESS,   // A read/write view of the file is mapped. 
		0,
		0,
		100						// Number Of Bytes To Map
	);
}

void waitSemaphore(HANDLE id)
{
	WaitForSingleObject(id, INFINITE);
}

void releaseSemaphore(HANDLE id)
{
	ReleaseSemaphore(id, 1, NULL);
}

void printStr()
{
	HANDLE handle, memory;
	char buff[100] = { 0 };
	void* memstart;

	handle = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, semName);
	memory = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, memName);
	if (memory == NULL) 
	{
		printf("Last OpenFileMapping Error: %d", GetLastError());
		getch();
		return;
	}
	if (!(memstart = getSharedMemory(memory))) 
	{
		printf("Last getSharedMemory Error: %d", GetLastError());
		getch();
		return;
	}
	while (1)
	{
		waitSemaphore(handle);
		memcpy(buff, memstart, 50);
		if (strcmp(buff, "exit\n") == 0) 
		{
			CloseHandle(handle);
			CloseHandle(memory);
			exit(EXIT_SUCCESS);
		}
		printf(buff);
		releaseSemaphore(handle);
	}
}