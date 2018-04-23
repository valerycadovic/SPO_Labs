#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

class CriticalOperationsSet
{
	int threadNumber;
	CRITICAL_SECTION section;
	static const int COUNT = 10;
	HANDLE threads[COUNT];
	DWORD WINAPI printUniqueString(PVOID args);
	static DWORD WINAPI threadFunction(PVOID args);
public:
	CriticalOperationsSet();
	~CriticalOperationsSet();
	void addThread();
	void removeThread();
	void removeAll();
private:
	struct ThreadArgs 
	{
		CriticalOperationsSet *cl;
		bool canClose;
		int number;
	}* args[COUNT];

	HANDLE getThread(ThreadArgs *arg, PTHREAD_START_ROUTINE function);
	int currentThread;
};

