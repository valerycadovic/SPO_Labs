#include "CriticalOperationsSet.h"

CriticalOperationsSet::CriticalOperationsSet()
{
	//cout << "construct" << endl;
	this->threadNumber = 0;
	currentThread = -1;


	InitializeCriticalSection(&this->section);
}

CriticalOperationsSet::~CriticalOperationsSet()
{
	DeleteCriticalSection(&this->section);
}

void CriticalOperationsSet::removeAll()
{
	//cout << "remAll" << endl;
	
	for (int i = 0; i < this->threadNumber; i++)
	{
		this->args[i]->canClose = true;
	}
}

void CriticalOperationsSet::addThread() 
{
	if (this->COUNT == this->threadNumber) return;

	//cout << "added" << endl;

	args[this->threadNumber] = new ThreadArgs();
	args[this->threadNumber]->canClose = false;
	args[this->threadNumber]->cl = this;
	args[this->threadNumber]->number = this->threadNumber;

	threads[this->threadNumber] = getThread(args[this->threadNumber], CriticalOperationsSet::threadFunction);
	this->threadNumber++;
}

DWORD WINAPI CriticalOperationsSet::printUniqueString(PVOID args)
{
	ThreadArgs* ta = reinterpret_cast<ThreadArgs*>(args);
	string str = "Thread";
	//cout << "print" << endl;
	while (!ta->canClose)
	{
		if (ta->number == currentThread)
			Sleep(200);		// nessecary to reduce competition
		//cout << "in a loop" << endl;
		EnterCriticalSection(&this->section);
		for (unsigned int i = 0; i < str.length(); i++)
		{
			cout << str[i];
			Sleep(200);
		}
		cout << ta->number << endl;

		LeaveCriticalSection(&this->section);

		currentThread = ta->number;
		//cout << this->section.RecursionCount << endl;
		//SwitchToThread();
		//std::this_thread::yield();
	}
	delete ta;
	return 0;
}

HANDLE CriticalOperationsSet::getThread(ThreadArgs *arg, PTHREAD_START_ROUTINE function)
{	
	SECURITY_ATTRIBUTES sa;
	sa.bInheritHandle = TRUE;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;

	//cout << "get" << endl;

	return CreateThread(&sa, 0, function, arg, 0, NULL);
}

DWORD WINAPI CriticalOperationsSet::threadFunction(PVOID args) 
{
	//cout << "static" << endl;
	return (reinterpret_cast<ThreadArgs*>(args))->cl->printUniqueString(args);
}

void CriticalOperationsSet::removeThread() 
{
	if (this->threadNumber == 0) return;
	//cout << "remove" << endl;
	this->threadNumber--;
	args[this->threadNumber]->canClose = true;
}