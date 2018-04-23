#include <iostream>
#include "CriticalOperationsSet.h"
#include <conio.h>

int main(int argc, char** argv)
{
	CriticalOperationsSet* set = new CriticalOperationsSet();

	while (true)
	{
		switch (_getch())
		{
		case '+':
			set->addThread();
			break;
		case '-':
			set->removeThread();
			break;
		case 'q':
			set->removeAll();
			goto END;
		}
	}
	END:
	cin.ignore();
	delete set;
	return 0;
}