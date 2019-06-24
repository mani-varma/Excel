#include "excelOperations.h"
#include<iostream>
#include<cstring>
using namespace std;

void run(struct cell** data,char* currFile=NULL)
{
	char* command = (char*)malloc(1001);
	cout << "\n>";
	gets(command);
	while (command[0] == ' ')
		command = command + 1;
	if ((command[0] == 'g' || command[0] == 'G') && checkCommand(command, "GET"))
	{
		int d = get(command, data);
		cout << d;
	}
	else if ((command[0] == 's' || command[0] == 'S') && checkCommand(command,"SET"))
		data = set(command, data);
	else if ((command[0] == 'p' || command[0] == 'P') && checkCommand(command, "PRINT"))
		print(data);
	else if ((command[0] == 'e' || command[0] == 'E') && checkCommand(command, "EXPORT"))
		export(command, data);
	else if ((command[0] == 'i' || command[0] == 'I') && checkCommand(command, "IMPORT"))
	{
		data = import(command);
		command = command + 6;
		while (command[0] == ' ')
			command = command + 1;
		currFile = command;
	}
	else if ((command[0] == 's' || command[0] == 'S') && checkCommand(command, "SAVE"))
		save(currFile, data);
	else if ((command[0] == 'e' || command[0] == 'E') && checkCommand(command, "EXIT"))
		return;
	else if ((command[0] == 'd' || command[0] == 'D') && checkCommand(command, "DELETE"))
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
			{
				data[i][j].value = 0;
				data[i][j].form = NULL;
			}
	}
	run(data,currFile);
}