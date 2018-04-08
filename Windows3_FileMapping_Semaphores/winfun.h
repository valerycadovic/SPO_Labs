#pragma once
#pragma warning (disable : 4996)		// allow obsolete C-functions
#pragma warning (disable : 4133)		
#pragma warning (disable : 4477)		// resolve winapi types conflicts

#include <Windows.h>
#include <stdio.h>
#include <conio.h>

HANDLE init(char* source, char* arg);		
HANDLE initSemaphore(char* str);			
HANDLE initSharedMemory(char* str);			
void* getSharedMemory(HANDLE id);			
void waitSemaphore(HANDLE id);
void releaseSemaphore(HANDLE id);
void printStr();
