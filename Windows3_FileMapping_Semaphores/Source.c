#pragma warning (disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "winfun.h"

int main(int argc, char** argv)
{
	char arg[100] = { 0 };								// command line agruments to be transferres to child process
	char str[50] = "";									
	HANDLE semaphore, memory;
	void* memstart;										// shared memory pointer

	if (argc > 1)										//-----------------------
	{													//
		puts("child: print");							// child process code
		printStr();										//
		return 0;										//
	}													//-----------------------

	semaphore = initSemaphore(argv[0]);					// create semaphore
	memory = initSharedMemory(argv[0]);					// create shared memory
	sprintf(arg, "%d %d", semaphore, memory);			// create command line
	memstart = getSharedMemory(memory);
	waitSemaphore(semaphore);
	HANDLE hChild = init(argv[0], arg);					// create child process
	puts("parent: scan");
	while (1)
	{
		fgets(str, 50, stdin);
		memcpy(memstart, str, 50);						// put string to shared memory
		releaseSemaphore(semaphore);
		if (strcmp("exit\n", str) == 0)					// exit condition
		{
			break;
		}
		waitSemaphore(semaphore);
	}
	system("pause");
	return 0;
}
