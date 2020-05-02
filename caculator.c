
#include "stack.h"
#include "calculator.h"
#include "quadHash.h"
#include "functions.h"
#include "list.h"

/*deal with a statement:
 *if the input is not a statment, return ERROE;else return 1 or 0;
 */
int bool(char* input, int start, int end) {
	int pos=start;
	while (pos < end && isspace(input[pos])) ++pos; //remove the space
	char a[100] = { 0 };
	int ap = 0;
	for (; pos < end; ++pos)
		a[ap++] = input[pos];// get the input in a array;

	if ((strchr(a, '=')) != NULL) { //deal with the "a = a>= b;" situation
		pos = strchr(a, '=') - a;
		if (a[pos - 1] != '>'&&a[pos - 1] != '<'&&a[pos - 1] != '='&&a[pos + 1] != '=')
			return ERROR;
	}

	if (strstr(a, "||") != NULL) { // the "||" operator
		int b1, b2;
		pos = strstr(input+start, "||") - input; //get the position of the operator;
		b1 = bool(input, start, pos);
		b2 = bool(input, pos + 2, end);
		if (b1 != ERROR && b2 != ERROR) {
			if (b1 || b2) {
				printf("true\n");
				return 1;
			}
			else {
				printf("false\n");
				return 0;
			}
		}
		else return ERROR;
	}
	else if (strstr(a, "&&") != NULL) { // the "&&" operator
		int b1, b2;
		pos = strstr(input+start, "&&") - input; //get the position of the operator;
		b1 = bool(input, start, pos);
		b2 = bool(input, pos + 2, end);
		if (b1 != ERROR && b2 != ERROR)
			if (b1 && b2) {
				printf("true\n");
				return 1;
			}
			else {
				printf("false\n");
				return 0;
			}
		else return ERROR;
	}
	else if (strstr(a, "!=") != NULL) { // the "!=" operator
		double x1, x2;
		pos = strstr(input+start, "!=") - input; //get the position of the operator;
		x1 = expression(input, start, pos);
		x2 = expression(input, pos + 2, end);
		if (x1 != x2) {
			printf("true\n");
			return 1;
		}
		else {
			printf("false\n");
			return 0;
		}
	}
	else if (strstr(a, "==") != NULL) { // the "==" operator
		double x1, x2;
		pos = strstr(input+start, "==") - input; //get the position of the operator;
		x1 = expression(input,start, pos); //process the expression; 
		x2 = expression(input,pos + 2, end);
		if (x1 == x2) {
			printf("true\n");
			return 1;
		}
		else {
			printf("false\n");
			return 0;
		}
	}
	else if (strstr(a, ">=") != NULL) { // the ">=" operator
		double x1, x2;
		pos = strstr(input+start, ">=") - input;
		x1 = expression(input,start, pos); //process the expression; 
		x2 = expression(input,pos + 2, end);
		if (x1 >= x2) {
			printf("true\n");
			return 1;
		}
		else {
			printf("false\n");
			return 0;
		}
	}
	else if (strstr(a, "<=") != NULL) { // the "<=" operator
		double x1, x2;
		pos = strstr(input+start, "<=") - input;
		x1 = expression(input,start, pos); //process the expression; 
		x2 = expression(input,pos + 2, end);
		if (x1 <= x2) {
			printf("true\n");
			return 1;
		}
		else {
			printf("false\n");
			return 0;
		}
	}
	else if (strchr(a, '<') != NULL) { // the "<" operator
		double x1, x2;
		pos = strchr(input+start, '<') - input;
		x1 = expression(input,start, pos);
		x2 = expression(input,pos + 1, end); //process the expression; 
		if (x1 < x2) {
			printf("true\n");
			return 1;
		}
		else {
			printf("false\n");
			return 0;
		}
	}
	else if (strchr(a, '>') != NULL) { // the ">" operator
		double x1, x2;
		pos = strchr(input+start, '>') - input;
		x1 = expression(input,start, pos);
		x2 = expression(input,pos + 1, end);
		if (x1 > x2) {
			printf("true\n");
			return 1;
		}
		else {
			printf("false\n");
			return 0;
		}
	}
	return ERROR; //if not match the operator above, then it's not a statment;
}

/*deal with a sentense, include defining functions,if or while statement*/
double sentence(char* input,int start, int end) {
	int pos = start;
	int pre_pos;
	double stat = PASS;

	while (pos < end && isspace(input[pos])) ++pos;//remove the space
	char a[100] = { 0 };//the array for matching the if, while , and define;
	int ap = 0;//the postion in array a;
	for (; pos < end; ++pos)//get the string from the input to the a array;
		a[ap++] = input[pos];

	pos = start;
	while (pos < end && isspace(input[pos])) ++pos;//remove the space
	if (pos == end)	return stat;// if it's an empty sentence;


	/* "if" block*/
	if (input[pos]=='i'&&input[pos+1]=='f') { 
		pos = pos + 2; //skip the "if"
		while (pos < end && isspace(input[pos])) ++pos;//remove the space

		/*match the begin and end of the if statment*/
		if (input[pos] != '('){ // if no match '('
			printf("(...?: if syntax error\n");
			return ERROR;
		}
		else
			++pos; // skip the '('
		pre_pos = pos;
		while (pos<end&&input[pos] != ')')	++pos;
		if (input[pos] != ')'){ // if no match '('
			printf(")...?: if syntax error\n");
			return ERROR;
		}

		/*if the statment is true;*/
		if (bool(input, pre_pos, pos) == 1) {

			/*fine the begin of the if block;*/
			pos++;
			while (pos < end && isspace(input[pos])) ++pos;//remove the space
			if (input[pos] != '{') {// if no match '{'
				printf("{...?: if syntax error\n");
				return ERROR;
			}

			/*fine the end of the if block;*/
			pos++;
			int fin = end - 1;
			while (fin > pos && input[fin] != '}')	--fin;// find the '}' from brack to front;
			if (fin==pos && input[fin] != '}') {// if no match '{'
				printf("}...?: if syntax error\n");
				return ERROR;
			}

			/*process the if block;*/
			stat=block(input, pos, fin);
		}
	}


	/*function define block;*/
	else if (strstr(a, "define") == a) {

		/*find the begin of the function name define*/
		pos = pos + 6; //skip the "define"
		char func_name[30];
		int fn_pos = 0;//the position of function name string;
		while (pos < end && isspace(input[pos])) ++pos;//remove the space
		if (input[pos] != '(') { // if no match '('
			printf("(...?: define syntax error\n");
			return ERROR;
		}
		++pos;// skip the '('

		/*get the name of the function*/
		while (pos < end && isspace(input[pos])) ++pos;//remove the space
		while (pos < end&&input[pos] != ')'&&isalpha(input[pos]))//get the function name in the string;	
			func_name[fn_pos++] = input[pos++];
		func_name[fn_pos] = 0;

		/*find the end of the function name define*/
		while (pos < end&&input[pos] != ')') ++pos;
		if (input[pos] != ')') { // if no match '('
			printf(")...?: define syntax error\n");
			return ERROR;
		}
		++pos;// skip the ')'

		/*match the function block and write it into the funtion buffer*/
		while (pos < end && isspace(input[pos])) ++pos;//remove the space
		if (input[pos] != '{') {// if no match '{'
			printf("{...?: define syntax error\n");
			return ERROR;
		}
		printf("define function:%s\n", func_name);
		functions[function_pos++] = ' ';
		int fblock_start = function_pos;
		charstack s = newcharstack();//match the '{' and '}';
		pushchar(s, '{');
		while (pos < end && !csempty(s)) {
			functions[function_pos++] = input[pos];
			if (input[pos] == '{')	pushchar(s, '{');
			else if (input[pos] == '}')	popchar(s);
			++pos;
		}
		if (pos==end && input[pos-1] != '}') {// if no match '}'
			printf("}...?: define syntax error\n");
			return ERROR;
		}
		functions[function_pos++] = ' ';
		int fblock_end = function_pos;

		/*insert the function name and the element into the name space*/
		ElementType Elem;
		Elem.type = self;
		Elem.value1 = fblock_start; //store the begin pos of function block in the value1;
		Elem.value2 = fblock_end; //store the end pos of function block in the value2;
		Elem.f = &fun_; //use the c function "fun_" to simulate the function process;
		L = Insert_list(L, Elem);
		stat = Insert(func_name, &L->Data, T->Data);//insert the function element;
	}


	/* "while" block*/
	else if (strstr(a, "while") == a) {
		pos = pos + 5;

		/*match the begin and end of the while statment*/
		while (pos < end && isspace(input[pos])) ++pos;//remove the space
		if (input[pos] != '(') { // if no match '('
			printf("(...?: while syntax error\n");
			return ERROR;
		}
		else
			++pos; // skip the '('
		pre_pos = pos;
		while (pos < end&&input[pos] != ')')	++pos;
		if (input[pos] != ')') { // if no match '('
			printf(")...?: while syntax error\n");
			return ERROR;
		}
		int np = pos;//store the end postion of statment, and the begin position of while block is np+1;

		/*if the statment is true;*/
		while (bool(input, pre_pos, np) == 1) {
			pos = np+1;

			while (pos < end && isspace(input[pos])) ++pos;//remove the space
			if (input[pos] != '{') {// if no match '{'
				printf("{...?: if syntax error\n");
				return ERROR;
			}

			/*fine the end of the if block;*/
			pos++;
			int fin = end - 1;
			while (fin > pos && input[fin] != '}')	--fin;// find the '}' from brack to front;
			if (fin==pos&&input[fin] != '}') {// if no match '{'
				printf("}...?: if syntax error\n");
				return ERROR;
			}

			/*process the if block;*/
			stat = block(input, pos, fin);
		}
	}


	/*process the situation that the sentence is a single statment*/
	else if ((stat = bool(input, pos, end)) != ERROR) {
		if (stat == 1)	printf("the statement is true\n");
		else if(stat==0)  printf("the statement is false\n");
	}


	/*porcess the assignment sentence*/
	else if (strchr(a, '=') != NULL) {
		double x;
		pos = strchr(input+start, '=') - input;
		ElementType Elem,*pele;

		/*if the assignment is a string, get the string range */
		int str_begin=pos+1, str_end=pos+1;
		while (str_begin < end&&input[str_begin] != '\"')	++str_begin;
		str_end = str_begin+1;
		while (str_end < end&&input[str_end] != '\"')	++str_end;

		/*if the string range is not empty, it's a string*/
		if (str_end != str_begin && str_end < end) {
			Elem.type = string;
			Elem.f = malloc(sizeof(char)*(str_end - str_begin));
			++str_begin;
			int str_pos = 0;
			while (str_begin < str_end)	((char*)Elem.f)[str_pos++] = input[str_begin++];//get in the string;
			((char*)Elem.f)[str_pos] = 0;
			x = strlen(((char*)Elem.f));// store the string length
		}

		/*if it's a statment*/
		else if ((stat = bool(input, pos+1, end)) != ERROR) {
			x = stat;
			Elem.type = boolean;
		}

		/*if it's a number*/
		else {
			x = expression(input,pos+1, end);
			Elem.type = var;
		}

		Elem.value2 = Elem.value1 = x;
		if (Elem.type != string)
			Elem.f = NULL;
		
		/*get the var name*/
		char new[30] = { 0 };
		int i=start,n=0;
		while (i < pos && isspace(input[i]))	++i;
		while (i<pos && isalpha(input[i]))	new[n++] = input[i++];//get in the function mane;
		new[n] = 0;

		//if it's an array type;
		if (((pele = find_Elem(new)) != NULL) && pele->type == array) {
			while (i < pos && (input[i] == ' ' || input[i] == '\n' || input[i] == '\t'))	++i;
			if (input[i] == '[') {
				++i;
				int pre_pos = i;
				while (input[i] != ']')	++i;
				if (input[i] == ']') {
					int index = expression(input, pre_pos, i);
					((double*)pele->f)[index] = x;
					stat = 1;
					return stat;
				}
				else
					printf("wrong format!\n");
			}
			else {
				printf("update: %s\n", new);
				*pele = Elem;
			}
		}
		//if the var name is exist;
		else if (pele != NULL) {
			printf("update: %s\n", new);
			*pele = Elem;
		}
		else {
			printf("define: %s\n", new);
			L = Insert_list(L, Elem);
			stat=Insert(new,&L->Data,T->Data);
		}
	}


	/* the sentence is a block;*/
	else if (input[pos] == '{') {
		int block_start = pos + 1;
		charstack s = newcharstack();
		pushchar(s, '{');
		while (pos < end && !csempty(s)) {
			++pos;
			if (input[pos] == '{')	pushchar(s, '{');
			else if (input[pos] == '}')	popchar(s);
		}
		if (pos == end && input[pos] != '}') {// if no match '}'
			printf("}...?: block syntax error\n");
			return ERROR;
		}
		int block_end = pos;
		stat = block(input, block_start, block_end);
	}


	/*if the sentence is in the function and with a return*/
	else if (strstr(a, "return") == a) {
		pos = pos + 6;
		double return_num;
		return_num = expression(input, pos, end);
		return return_num;
	}


	/*if the sentence is a simple statement*/
	else{
		ans = expression(input,pos, end);
		if(ans!=ERROR)
			printf("answer = %lf\n", ans);
		return PASS;
	}

	return stat;
}


double block(char* input, int start, int end) {
	int pos = start, before;
	double stat=PASS;
	while (pos < end && isspace(input[pos])) ++pos;//remove the space

	while (pos < end) { //get in the sentences one by one;
		while (pos < end && isspace(input[pos])) ++pos;//remove the space
		before = pos;
		while (pos < end && input[pos] != '{' && input[pos] != ';')	++pos; //get the sentence;
		if (input[pos] == '{') { //if find a block in the sentence;
			charstack s = newcharstack();
			pushchar(s, '{');
			while (pos < end && !csempty(s)) {
				++pos;
				if (input[pos] == '{')	pushchar(s, '{');
				else if (input[pos] == '}')	popchar(s);
			}
			if (pos == end && input[pos] != '}') {// if no match '}'
				printf("}...?: block syntax error\n");
				return ERROR;
			}
		}
		while (pos < end && input[pos] != ';')	++pos;
		stat=sentence(input, before, pos); //process the sentence;
		if(input[pos]==';') ++pos;
	}
	return stat;
}

/*the main function of the programme;*/
int main()
{
	ini_functions();
	printf("Interpreter v1.0:\n");
	printf("type help() for further information, enter exit() to exit:\n");
	printf("multiline input can use ':' to start and ':' to finish:\n");
	printf("( you can also enter brainfuck() to use this language _(:§Ù¡¹¡Ï)_ )\n");
	printf("****************************************************************\n");
	while (1)
	{	
		function_pos = 0;
		char input[1000] = { 0 };/*input buffer*/
		int pos=0;
		printf(">>> ");
		gets(input);
		if (input[0] == ':') {
			pos = strlen(input);
			while ((input[pos++] = getchar()) != ':');
			input[pos] = 0;
			block(input, 1, strlen(input)-1);
		}
		else
			block(input, 0, strlen(input));
	}
	return 0;
}
