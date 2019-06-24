#include<iostream>
using namespace std;
#include "excelRun.h"
using namespace std;
int main()
{
	struct cell** data = (struct cell**)malloc(sizeof(struct cell*) * 10);
	for (int i = 0; i < 10; i++)
		data[i] = (struct cell*)malloc(sizeof(struct cell) * 10);
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			data[i][j].value = 0;
			data[i][j].form = NULL;
		}
	run(data);
	system("pause");
	return 0;
}