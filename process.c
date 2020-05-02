/*this source file include the functions used to calculate the expressions*/
#include "stack.h"
#include "calculator.h"
#include "quadHash.h"
#include "functions.h"
#include "list.h"

/*----------private functions-------------*/

/*proccess the functions or varities;*/
int alpha(char* input, char ch, int p, char* mid, int i);//变量、函数的寻址入口

/*convert a bunch of figures in the expression into double type,flag decides the sign*/
double todigit(char a, int flag, char* mid, int i);//从中缀表达式中获得数字

/*Processing each number and computing infix expressions*/
double caculate(char* mid);//计算中缀表达式

/*proccess the functions or varities;*/
int alpha(char* input, char ch, int p, char* mid, int i) {
	int dif = p;
	char c[30];
	int j = 1;

	/*get the var name in;*/
	c[0] = ch;
	while (isalpha(ch = input[p++]))	c[j++] = ch;
	c[j] = '\0';
	
	/*find the var in the name space*/
	double  x = 0;
	ElementType *pele=NULL;
	if (strcmp(c, "ans") == 0)
		x = ans;
	else if ((pele = find_Elem(c)) != NULL) {
		/*switch the var type;*/
		if (pele->type == function)//system function
			x = (*(afunc)pele->f)(input, p);
		else if (pele->type == boolean) { //boolean tpye
			x = pele->value2;
			if (x == 1)
				printf("%s is true\n",c);
			else
				printf("%s is false\n",c);
		}
		else if (pele->type == self) { //self functions
			x = (*(self_fun)pele->f)(input, p,pele);
		}
		else if (pele->type == string) { //string;
			printf("%s\n", (char*)pele->f);
			while (input[p] && p < dif + 30 && isspace(input[p]))	++p;
			if (input[p] == '[' || ch=='[') { //the string is used by []
				if(input[p] == '[')
					++p;
				int pre_pos = p, index;
				while (input[p] != ']')	++p;
				index = expression(input, pre_pos, p);
				++p;
				if (index >= pele->value2 || index < 0) {
					printf("string out of range\n");
					x = 0;
				}
				else {
					x = ((char*)pele->f)[index];
					printf("the char: %c\n", (char)x);
				}
			}else
				x = pele->value2; //the string is used as a whole
		}
		else if (pele->type == array) { //array
			while (input[p] && p < dif + 30 && isspace(input[p]))	++p;
			if (input[p] == '[' || ch == '[') { //the string is used by []
				if (input[p] == '[')
					++p;
				int pre_pos = p, index;
				while (input[p] != ']')	++p;
				index = expression(input, pre_pos, p);
				++p;
				if (index >= pele->value2 || index < 0) {
					printf("array out of range\n");
					x = 0;
				}
				else
					x = ((double*)pele->f)[index];
			}
			else {
				printf("%s is an array:\n", c);
				for (int k = 0; k < pele->value2; ++k) {
					printf(" %lf", ((double*)pele->f)[k]);
				}
				putchar('\n');
				x = pele->value2;
			}
		}
		else {
			x = pele->value1;
		}
	}
	else {
		printf("ERROR: undefined character or functions\n");
		return ERROR;
	}
	/*add the result into fuunctions store,infix expression replaced by character*/
	mid[i++] = ' ';
	sprintf(mid + i, "%f", x);
	if (ch == '(') {
		while (input[p] != ')')	++p;
		++p;
	}
	else
		--p;
	return p - dif;
}

/*For input character-by-character processing and generating infix expressions, the number position remains unchanged,while the operator position is changed*/
double expression(char* input,int start, int end)
{
	char mid[200] = { 0 };/*post operation*/
	int i = 0;
	double x;
	int p = start;
	charstack a = newcharstack();
	char ch;
	/*flag is used to indicate if a 'ch' before is an operator,to support signed figures*/
	int flag = 1;
	/*multiple condition judgment*/
	while (p < end && (ch = input[p++]) != '\0')
	{
		/*figure:sign before it*/
		if (isdigit(ch) && flag)
		{
			if (flag == 1)
				mid[i++] = ' ';
			mid[i++] = ch;
			flag = 0;
		}
		/*figure:no sign or decimal*/
		else if (isdigit(ch) || ch == '.')
			mid[i++] = ch;
		/*sign:positive */
		else if (flag && (ch == '-' || ch == '+'))
		{
			mid[i++] = ' ';
			mid[i++] = ch;
			flag = 2;
		}
		/*operation:multiplication and division,put it inside*/
		else if (ch == '*' || ch == '/' || ch=='%')
		{
			flag = 1;
			if (csempty(a))
				pushchar(a, ch);
			else if (cstop(a) == '+' || cstop(a) == '-' || cstop(a) == '(')
				pushchar(a, ch);
			else {
				mid[i++] = ' ';
				mid[i++] = cstop(a);
				popchar(a);
				pushchar(a, ch);
			}
		}
		/*operation:addtion and subtraction,pop multiplication and division*/
		else if (ch == '+' || ch == '-')
		{
			flag = 1;
			if (csempty(a) || cstop(a) == '(')
				pushchar(a, ch);
			else {
				while (!csempty(a) && (cstop(a) != '+'&&cstop(a) != '-'&&cstop(a) != '('))
				{
					mid[i++] = ' ';
					mid[i++] = cstop(a);
					popchar(a);
				}
				if (!csempty(a) && cstop(a) != '(')
				{
					mid[i++] = ' ';
					mid[i++] = cstop(a);
					popchar(a);
				}
				pushchar(a, ch);
			}
		}
		/*operation:bra,push in*/
		else if (ch == '(')
		{
			pushchar(a, ch);
		}
		/*operation:ket,pop until meet bra*/
		else if (ch == ')')
		{
			while (!csempty(a) && cstop(a) != '(')
			{
				mid[i++] = ' ';
				mid[i++] = cstop(a);
				popchar(a);
			}
			if (!csempty(a) && cstop(a) == '(')
				popchar(a);
		}
		/*character,meet function*/
		else if (isalpha(ch))
		{
			int dif;
			if ((dif = alpha(input,ch, p, mid, i)) == ERROR)
				return ERROR;
			else
				p += dif;
			flag = 0;
			while (mid[i] != '\0')	++i;
			mid[i++] = ' ';
		}
	}
	/*if the operation stack is not NULL,continue to pop*/
	while (!csempty(a))
	{
		mid[i++] = ' ';
		mid[i++] = cstop(a);
		popchar(a);
	}
	printf("mid[]: %s\n", mid);
	x=caculate(mid);
	if (x == ERROR) {
		printf("calculate error!\n");
		return ERROR;
	}
	else
		return x;
}

/*convert a bunch of figures in the expression into double type,flag decides the sign*/
double todigit(char a, int flag, char* mid, int i)
{
	double x = a - '0';
	++i;
	/*processing integer*/
	while (mid[i] != '\0' && mid[i] != ' ' && mid[i] != '.')
		x = x * 10 + mid[i++] - '0';
	if (mid[i] == '.')
	{
		/*processing decimal*/
		++i;
		int count1 = 1;
		while (mid[i] != '\0'&&mid[i] != ' ')
		{
			x = x + (mid[i] - '0') / pow(10, count1);
			++i;
			++count1;
		}
	}
	if (flag == 2)
		x = -x;
	//printf("%f ", x);
	return x;
}

/*Processing each number and computing infix expressions*/
double caculate(char* mid)
{
	char op = 0;
	/*operate on number stack*/
	doublestack num = newdoublestack();
	int flag = 0;
	int i = 1;
	double x;
	while (mid[i] != '\0')
	{
		/*operate when meeting blanks*/
		if (mid[i] == ' '&&op)
		{
			flag = 0;
			double x1, x2;
			if (dsempty(num))
				return ERROR;
			x1 = dstop(num);
			popdouble(num);
			if (dsempty(num))
				return ERROR;
			x2 = dstop(num);
			popdouble(num);
			switch (op)
			{
			case '+':x = x2 + x1; break;
			case '-':x = x2 - x1; break;
			case '*':x = x2 * x1; break;
			case '/':
				if (x1 == 0)
				{
					printf("ERROR: dividing number can not be zero\n");
					return ERROR;
					break;
				}
				else {
					x = x2 / x1; break;
				}
			case '%':
				if (x1 == 0)
				{
					printf("ERROR: dividing number can not be zero\n");
					return ERROR;
					break;
				}
				else {
					x = (int)x2 % (int)x1; break;
				}
			}
			pushdouble(num, x);
			op = 0;
		}
		/*convert signed figure*/
		else if (flag&&isdigit(mid[i]))
		{
			x = todigit(mid[i], flag, mid, i);
			while (mid[i] != '\0'&&mid[i] != ' ')	++i;
			pushdouble(num, x);
			flag = 0;
			op = 0;
		}
		/*convert unsigned figure*/
		else if (isdigit(mid[i]))
		{
			x = todigit(mid[i], 0, mid, i);
			while (mid[i] != '\0'&&mid[i] != ' ')	++i;
			pushdouble(num, x);
		}
		/*mark op for operators*/
		else if (mid[i] == '+' || mid[i] == '-' || mid[i] == '/' || mid[i] == '*' || mid[i]=='%')
		{
			op = mid[i];
			if (mid[i] == '+')
				flag = 1;
			else if (mid[i] == '-')
				flag = 2;
		}
		/*do nothing when meets blanks*/
		/*else if (mid[i] == ' ');*/
		i++;
	}
	/*calculate the last time if there remains any calculation undone*/
	if (op != '\0')
	{
		double x1, x2;
		if (dsempty(num))
			return ERROR;
		x1 = dstop(num);
		popdouble(num);
		if (dsempty(num))
			return ERROR;
		x2 = dstop(num);
		popdouble(num);
		switch (op)
		{
		case '+':x = x2 + x1; break;
		case '-':x = x2 - x1; break;
		case '*':x = x2 * x1; break;
		case '/':
			if (x1 == 0)
			{
				printf("ERROR: dividing number can not be zero\n");
				return ERROR;
				break;
			}
			else {
				x = x2 / x1; break;
			}
		case '%':
			if (x1 == 0)
			{
				printf("ERROR: dividing number can not be zero\n");
				return ERROR;
				break;
			}
			else {
				x = (int)x2 % (int)x1; break;
			}
		}
		pushdouble(num, x);
	}
	if (dsempty(num))
		return ERROR;
	x = dstop(num);
	popdouble(num);
	if (!dsempty(num))
		return ERROR;
	return x;
}
