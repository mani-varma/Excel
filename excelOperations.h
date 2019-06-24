#include<iostream>
using namespace std;
struct cell
{
	int value;
	char* form;
};

struct expNode
{
	char* var;
	struct expNode* next;
};


bool checkCommand(char* userCommand, char* originalCommand)
{
	int i;
	while (userCommand[0] == ' ')
		userCommand = userCommand + 1;
	for (i = 0;(userCommand[i] != ' '||userCommand[i]!='\0')&&originalCommand[i] != '\0'; i++)
		if (toupper(userCommand[i]) != originalCommand[i])
			return false;
	if ((userCommand[i] != ' ' || userCommand[i] != '\0') && originalCommand[i] == '\0')
		return true;
	else
		return false;
}

char* toChar(char* str, int value)
{
	char* newStr;
	int len;
	for (len = 0; str[len] != '\0'; len++);
	int dig=0,v1=value;
	do
	{
		dig++;
		v1 /= 10;
	} while (v1 > 0);
	newStr = (char*)malloc(len + dig+1);
	for (int i = 0; i < len; i++)
		newStr[i] = str[i];
	newStr[len + dig] = '\0';
	for (int i = len + dig - 1;i>=len; i--, value /= 10)
		newStr[i]= '0' + value % 10;
	return newStr;
}

int toInt(char* str)
{
	int n = 0;
	while (str[0]<'0' || str[0]>'9')
		str = str + 1;
	while (str[0] != '\0' && (str[0] >= '0'&&str[0] <= '9'))
	{
		n = n * 10 + int(str[0]) - int('0');
		str = str + 1;
	}
	return n;
}

bool checkIfValid(char* str, char* block, struct cell** data)
{
	for (int i = 0; str[i+1] != '\0'; i++)
	{
		if (str[i] >= 'A'&&str[i] <= 'Z')
		{
			if (str[i] == block[0] && str[i + 1] == block[1] && (block[2] == ' ' || block[2] == '=' || (str[i + 2] != '\0'&&str[i + 2] == block[2])))
				return false;
			else
			{
				if (data[toInt(str + i + 1) - 1][int(str[i]) - int('A')].form != NULL)
				{
					bool flag = checkIfValid(data[toInt(str + i + 1) - 1][int(str[i]) - int('A')].form, block, data);
					if (flag == false)
						return false;
				}
			}
		}
	}
	return true;
}

int get(char* pos, struct cell** data);

int evalPostFix(struct expNode* root, struct cell ** data)
{
	char** stack = (char**)malloc(sizeof(char*) * 20);
	for (int i = 0; i < 20; i++)
		stack[i] = (char*)malloc(10);
	int pos = -1;
	while (root != NULL)
	{
		if (root->var[0] == '+' || root->var[0] == '-' || root->var[0] == '*' || root->var[0] == '/' || root->var[0] == '%')
		{
			char* op1 = stack[pos--];
			int n1, n2;
			if (op1[0] >= 'A'&&op1[0] <= 'Z')
			{
				char* getOp1 = (char*)malloc(strlen(op1) + 4);
				for (int i = 0; i < 3; i++)
					getOp1[i] = '-';
				for (int i = 3, j = 0; i <strlen(op1) + 3; i++, j++)
					getOp1[i] = op1[j];
				getOp1[strlen(op1) + 3] = '\0';
				n1 = get(getOp1, data);
			}
			else
				n1 = toInt(op1);
			char* op2 = stack[pos--];
			if (op2[0] >= 'A'&&op2[0] <= 'Z')
			{
				char* getOp2 = (char*)malloc(strlen(op2) + 4);
				for (int i = 0; i < 3; i++)
					getOp2[i] = '-';
				for (int i = 3, j = 0; i < strlen(op2) + 6; i++, j++)
					getOp2[i] = op2[j];
				getOp2[strlen(op2) + 6] = '\0';
				n2 = get(getOp2, data);
			}
			else
				n2 = toInt(op2);
			switch (root->var[0])
			{
			case '+':stack[++pos] = toChar("", n2 + n1);
				break;
			case '-':stack[++pos] = toChar("", n2 - n1);
				break;
			case '*':stack[++pos] = toChar("", n2*n1);
				break;
			case '/':stack[++pos] = toChar("", n2 / n1);
				break;
			case '%':stack[++pos] = toChar("", n2%n1);
				break;
			}
		}
		else
			stack[++pos] = root->var;
		root = root->next;
	}
	return toInt(stack[0]);
}

struct expNode* postfixAsLL(char* form)
{
	struct expNode* root1 = (struct expNode*)malloc(sizeof(struct expNode));
	root1 = NULL;
	struct expNode* prevNode = (struct expNode*)malloc(sizeof(struct expNode));
	int p = -1;
	char* stack = (char*)malloc(1000);
	while (form[0] != '\0')
	{
		struct expNode* newNode = (struct expNode*)malloc(sizeof(struct expNode));
		newNode->var = (char*)malloc(10);
		if (form[0] >= '0'&&form[0] <= '9')
		{
			struct expNode* newNode = (struct expNode*)malloc(sizeof(struct expNode));
			newNode->var = (char*)malloc(10);
			newNode->next = NULL;
			int i = 0;
			while (form[i] >= '0'&&form[i] <= '9')
			{
				newNode->var[i] = form[i];
				i++;
			}
			newNode->var[i] = '\0';
			if (root1 == NULL)
			{
				root1 = newNode;
				prevNode = newNode;
			}
			else
			{
				prevNode->next = newNode;
				prevNode = newNode;
			}
			form = form + i;
		}
		else if (form[0] >= 'A' && form[0] <= 'Z')
		{
			struct expNode* newNode = (struct expNode*)malloc(sizeof(struct expNode));
			newNode->var = (char*)malloc(10);
			newNode->next = NULL;
			int i = 0;
			while ((form[i] >= 'A'&&form[i] <= 'Z') || (form[i] >= '0'&&form[i] <= '9'))
			{
				newNode->var[i] = form[i];
				i++;
			}
			newNode->var[i] = '\0';
			if (root1 == NULL)
			{
				root1 = newNode;
				prevNode = newNode;
			}
			else
			{
				prevNode->next = newNode;
				prevNode = newNode;
			}
			form = form + i;
		}
		else if (form[0] == '+' || form[0] == '-')
		{
			if (p == -1)
			{
				stack[++p] = form[0];
				form = form + 1;
			}
			else
			{
				while (p > -1 && stack[p] != '(')
				{
					struct expNode* newNode = (struct expNode*)malloc(sizeof(struct expNode));
					newNode->var = (char*)malloc(2);
					newNode->next = NULL;
					newNode->var[0] = stack[p--];
					newNode->var[1] = '\0';
					prevNode->next = newNode;
					prevNode = newNode;
				}
				stack[++p] = form[0];
				form = form + 1;
			}
		}
		else if (form[0] == '*' || form[0] == '/' || form[0] == '%')
		{
			if (p == -1)
			{
				stack[++p] = form[0];
				form = form + 1;
			}
			else
			{
				while (p > -1 && (stack[p] == '*' || stack[p] == '/' || stack[p] == '%'))
				{
					struct expNode* newNode = (struct expNode*)malloc(sizeof(struct expNode));
					newNode->var = (char*)malloc(2);
					newNode->next = NULL;
					newNode->var[0] = stack[p--];
					newNode->var[1] = '\0';
					prevNode->next = newNode;
					prevNode = newNode;
				}
				stack[++p] = form[0];
				form = form + 1;
			}
		}
		else if (form[0] == ')')
		{
			while (stack[p] != '(')
			{
				struct expNode* newNode = (struct expNode*)malloc(sizeof(struct expNode));
				newNode->var = (char*)malloc(2);
				newNode->next = NULL;
				newNode->var[0] = stack[p--];
				newNode->var[1] = '\0';
				prevNode->next = newNode;
				prevNode = newNode;
			}
			p--;
			form = form + 1;
		}
		else if (form[0] == '(')
		{
			stack[++p] = form[0];
			form = form + 1;
		}
		else
			form = form + 1;
	}
	while (p > -1)
	{
		struct expNode* newNode = (struct expNode*)malloc(sizeof(struct expNode));
		newNode->var = (char*)malloc(2);
		newNode->next = NULL;
		newNode->var[0] = stack[p--];
		newNode->var[1] = '\0';
		prevNode->next = newNode;
		prevNode = newNode;
	}
	return root1;
}


int eval(char* form, struct cell** data)
{
	struct expNode* root = (struct expNode*)malloc(sizeof(struct expNode));
	root = postfixAsLL(form);
	return evalPostFix(root, data);
}



int get(char* pos, struct cell** data)
{
	try
	{
		while (pos[0] == ' ')
			pos = pos + 1;
		pos = pos + 3;
		while (pos[0] == ' ')
			pos = pos + 1;
		pos[0] = toupper(pos[0]);
		if (data[toInt(pos + 1) - 1][int(pos[0]) - int('A')].form == NULL)
			return data[toInt(pos + 1) - 1][int(pos[0]) - int('A')].value;
		else
			return eval(data[toInt(pos + 1) - 1][int(pos[0]) - int('A')].form, data);
	}
	catch (exception){
		return 0;
	}
}



struct cell** set(char* pos, struct cell** data)
{
	try
	{
		while (pos[0] == ' ')
			pos = pos + 1;
		pos = pos + 3;
		while (pos[0] == ' ')
			pos = pos + 1;
		pos[0] = toupper(pos[0]);
		char* find = pos;
		while (find[0] != '=')
			find = find + 1;
		find = find + 1;
		while (find[0] == ' ')
			find = find + 1;
		int i = 0,f=0;
		for (; find[i] != '\0'; i++);
		i--;
		while (i >= 0)
		{
			if (find[i]<'0' || find[i]>'9')
			{
				f = 1;
				break;
			}
			i--;
		}
		if (f)
		{
			for (int i = 0; find[i] != '\0'; i++)
				find[i] = toupper(find[i]);
			if (checkIfValid(find, pos, data))
				data[toInt(pos + 1) - 1][int(pos[0]) - int('A')].form = find;
			else
				cout << "It is Invalid";
		}
		else
		{
			int val = toInt(find);
			data[toInt(pos + 1) - 1][int(pos[0]) - int('A')].value = val;
		}
	}
	catch (exception){}
	return data;
}


void print(struct cell** data)
{
	for (int i = -1; i < 10; i++)
	{
		for (int j = -1; j < 10; j++)
		{
			if (i == -1 && j == -1)
				cout << "  ";
			else if (i == -1)
				cout << char('A' + j) << "      ";
			else if (j == -1)
				cout << i + 1 << " ";
			else
			{
				if (data[i][j].form == NULL)
					cout << data[i][j].value<<"      ";
				else
					cout << eval(data[i][j].form, data)<<"      ";
			}
		}
		cout << "\n";
	}
}


void export(char* pos,struct cell** data)
{
	while (pos[0] == ' ')
		pos = pos + 1;
	pos = pos + 6;
	while (pos[0] == ' ')
		pos = pos + 1;
	int l = strlen(pos);
	char* c = (char*)malloc(l + 5);
	char* t = (char*)malloc(l + 5);
	FILE* csv;
	FILE* txt;
	for (int i = 0; i < l; i++)
		c[i] = t[i] = pos[i];
	c[l] = t[l] = '.';
	c[l + 1] = 'c';
	c[l + 2] = 's';
	c[l + 3] = 'v';
	t[l + 1] = t[l + 3] = 't';
	t[l + 2] = 'x';
	c[l + 4] = t[l + 4] = '\0';
	csv = fopen(c, "w+");
	txt = fopen(t, "w+");
	int f = 0;
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			char* str;
			if (i < 10)
			{
				str = (char*)malloc(6);
				for (int i = 0; i < 3; i++)
					str[i] = '-';
				str[3] = 'A' + j;
				str[4] = char('0') + i;
				str[5] = '\0';
			}
			else
			{
				str = (char*)malloc(7);
				for (int i = 0; i < 3; i++)
					str[i] = '-';
				str[3] = 'A' + j;
				str[4] = char('0') + i / 10;
				str[5] = char('0') + i % 10;
				str[6] = '\0';
			}

			fprintf(csv, "%d,", get(str, data));
			if (data[i - 1][j].form != NULL)
				fprintf(txt,"%s=%s\n",str + 3, data[i - 1][j].form);
		}
		fprintf(csv,"\n");
	}
	fclose(csv);
	fclose(txt);
}


struct cell** import(char* pos)
{
	while (pos[0] == ' ')
		pos = pos + 1;
	pos = pos + 6;
	while (pos[0] == ' ')
		pos = pos + 1;
	struct cell** data = (struct cell**)malloc(sizeof(struct cell*) * 10);
	for (int i = 0; i < 10; i++)
		data[i] = (struct cell*)malloc(sizeof(struct cell) * 10);
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			data[i][j].value = 0;
			data[i][j].form = NULL;
		}
	int l = strlen(pos);
	FILE* csv;
	FILE* txt;
	char* c = (char*)malloc(l + 5);
	char* t = (char*)malloc(l + 5);
	for (int i = 0; i < l; i++)
		c[i] = t[i] = pos[i];
	c[l] = t[l] = '.';
	c[l + 1] = 'c';
	c[l + 2] = 's';
	c[l + 3] = 'v';
	t[l + 1] = t[l + 3] = 't';
	t[l + 2] = 'x';
	c[l + 4] = t[l + 4] = '\0';
	csv = fopen(c, "r");
	txt = fopen(t, "r");
	for (int i = 1; i <= 10; i++)
	{
		char* n;
		for (int j = 0; j < 10; j++)
		{
			int d;
			char c;
			char* str;
			if (i < 10)
			{
				str = (char*)malloc(7);
				for (int x = 0; x < 3; x++)
					str[x] = '-';
				str[3] = 'A' + j;
				str[4] = char('0') + i;
				str[5] = '=';
				str[6] = '\0';
			}
			else
			{
				str = (char*)malloc(8);
				for (int i = 0; i < 3; i++)
					str[i] = '-';
				str[3] = 'A' + j;
				str[4] = char('0') + (i / 10);
				str[5] = char('0') + (i % 10);
				str[6] = '=';
				str[7] = '\0';
			}
			fscanf(csv,"%d",&d);
			fscanf(csv,"%c",&c);
			set(toChar(str,d),data);
		}
		fscanf(csv,"%c",&n);
	}
	char* readForm = (char*)malloc(1001);
	while (fscanf(txt, "%s", readForm) == 1)
	{
		int i;
		for (i = 0; readForm[i]!= '='; i++);
		data[toInt(readForm + 1) - 1][int(readForm[0]) - int('A')].form = readForm + i + 1;
		readForm = (char*)malloc(1001);
	}
	fclose(txt);
	fclose(csv);
	return data;
}



void save(char* currFile,struct cell** data)
{
	int len;
	for (len = 0; currFile[len] != '\0'; len++);
	char* fileName = (char*)malloc(len + 7);
	fileName[len + 7] = '\0';
	for (int i = 0; i < 6; i++)
		fileName[i] = '-';
	for (int i = 6, j = 0; i < len + 7; i++, j++)
		fileName[i] = currFile[j];
	export(fileName,data);
}