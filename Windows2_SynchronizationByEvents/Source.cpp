#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#define PROCESS_LIMIT 10
PROCESS_INFORMATION CreateDefaultProcess(char* path, char* cmdArg);
void PrintUniqueString(char* string);

PROCESS_INFORMATION processes[PROCESS_LIMIT];
char stringSet[PROCESS_LIMIT][10] = {
	{ "One" },{ "Two" },{ "Three" },{ "Four" },{ "Five" },{ "Six" },{ "Seven" },{ "Eight" },{ "Nine" },{ "Ten" }
};

int main(int argc, char** argv)
{
	
	if (argc >= 2)
	{
		HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "CanWriteEvent");
		char eventName[20];
		sprintf(eventName, " %s %s", argv[1], argv[2]);
		HANDLE hCloseEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventName);
		HANDLE hExitEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "CanExitEvent");
		
		while (TRUE)
		{
			WaitForSingleObject(hEvent, INFINITE);
			if (WaitForSingleObject(hCloseEvent, 100) == WAIT_OBJECT_0)
			{
				SetEvent(hEvent);
				exit(EXIT_SUCCESS);
			}

			ResetEvent(hEvent);
			PrintUniqueString(stringSet[atoi(argv[2])]);
			SetEvent(hEvent);
		}
	}
	else
	{
		char ch;
		int processCount = 0;
		HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, "CanWriteEvent");
		HANDLE hCloseEvents[PROCESS_LIMIT];
		HANDLE hExitEvent = CreateEvent(NULL, FALSE, FALSE, "CanExitEvent");
	
	AGAIN:

		do 
		{
			rewind(stdin);
			ch = _getch();
		}  while (ch != '+' && ch != '-' && ch != 'q');

		switch (ch)
		{
		case '+':
		{
			char buffer[20];
			sprintf(buffer, " %s %d", stringSet[processCount], processCount);
			hCloseEvents[processCount] = CreateEvent(NULL, FALSE, FALSE, buffer);
			processes[processCount] = CreateDefaultProcess(argv[0], buffer);
			processCount++;
			goto AGAIN;
		}
		case '-':
		{
			WaitForSingleObject(hEvent, INFINITE);
			SetEvent(hCloseEvents[--processCount]);
			SetEvent(hEvent);
			goto AGAIN;
		}
		default:
		{
			ResetEvent(hEvent);
			for (int i = 0; i < processCount; i++)
			{
				TerminateProcess(processes[i].hProcess, EXIT_FAILURE);
			}
			puts("\n\nI killed them. I killed them all.\nThey're dead, every single one of them. \nAnd not just the men, but the women and the children, too.\nThey're like animals, and I slaughtered them like animals.\n\n\t\t");
			PrintUniqueString("I HATE THEM.");
			_getch();
			ExitProcess(0);
		}
		}
	}

	return TRUE;
}

void PrintUniqueString(char* string)
{
	for (int i = 0; i < strlen(string); i++)
	{
		printf("%c", string[i]);
		Sleep(200);
	}
	puts("");
}

PROCESS_INFORMATION CreateDefaultProcess(char* path, char* cmdArg)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if (!CreateProcess(
		path, cmdArg,
		NULL, NULL,
		TRUE, NULL,
		NULL, NULL,
		&si, &pi
	))
	{
		puts("rfr");
	}
	return pi;
}

