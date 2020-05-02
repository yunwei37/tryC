

#include "quadHash.h"
#include "functions.h"
#include "stack.h"

extern void interpret(char *c);

/*pow(x,y)*/
double pow_(char* input,int p) {
	int dif = p;//record the begin position, in the format wrong case;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p;// skip the space
	double x1, x2;
	int pre_pos = p;
	while ( input[p] && p<dif+100 && input[p] != ',')	++p; //find the ','
	if (input[p] != ',') {
		printf("function , wrong format!\n");//if cannot find the ','
		return ERROR;
	}
	x1 = expression(input, pre_pos, p); //calculate x1;
	++p;
	pre_pos = p;
	while (input[p] && p < dif + 100 && input[p] != ')')	++p; //find the ')'
	if (input[p] != ')') {
		printf("function ')'wrong format!\n"); //if not found
		return ERROR;
	}
	x2 = expression(input, pre_pos, p); //calculate x2;
	return pow(x1, x2);
}

/*sin(x)*/
double sin_(char* input, int p) {
	double x1;
	int dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p; // skip the space
	int pre_pos = p;
	while (input[p] && p < dif + 100 && input[p] != ')')	++p; //find the ')
	if (input[p] != ')') {
		printf("function ')'wrong format!\n"); //if not found
		return ERROR;
	}
	x1 = expression(input, pre_pos, p); //calculate x1;
	return sin(x1);
}

/*cos(x)*/
double cos_(char* input, int p) {
	double x1;
	int dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p;
	int pre_pos = p;
	while (input[p] && p < dif + 100 && input[p] != ')')	++p;
	if (input[p] != ')') {
		printf("function ')'wrong format!\n");
		return ERROR;
	}
	x1 = expression(input, pre_pos, p);
	return cos(x1);
}

/*tan(x)*/
double tan_(char* input, int p) {
	double x1;
	int dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p;
	int pre_pos = p;
	while (input[p] && p < dif + 100 && input[p] != ')')	++p;
	if (input[p] != ')') {
		printf("function ')'wrong format!\n");
		return ERROR;
	}
	x1 = expression(input, pre_pos, p);
	return tan(x1);
}

/*abs(x)*/
double abs_(char* input, int p) {
	double x1;
	int dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p;
	int pre_pos = p;
	while (input[p] && p < dif + 100 && input[p] != ')')	++p;
	if (input[p] != ')') {
		printf("function ')'wrong format!\n");
		return ERROR;
	}
	x1 = expression(input, pre_pos, p);
	return fabs(x1);
}

/*sqrt(x)*/
double sqrt_(char* input, int p) {
	double x1;
	int dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p;
	int pre_pos = p;
	while (input[p] && p < dif + 100 && input[p] != ')')	++p;
	if (input[p] != ')') {
		printf("function ')'wrong format!\n");
		return ERROR;
	}
	x1 = expression(input, pre_pos, p);
	return sqrt(x1);
}

/*exit() or quit*/
double exit_(char* input, int p) {
	exit(0);
	return 0;
}

/*get a num from the input stream*/
double getnum_(char* input, int p) {
	//get a double number from the input stream;
	double x1;
	int dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p; // skip the space
	while (input[p] && p < dif + 100 && input[p] != ')')	++p; //find the ')
	if (input[p] != ')') {
		printf("function ')'wrong format!\n"); //if not found
		return ERROR;
	}
	scanf("%lf", &x1);
	return x1;
}

/*print a num to the screen;*/
double printnum_(char* input, int p) {
	//get a double number from the input stream;
	double x1;
	int dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p;
	int pre_pos = p;
	while (input[p] && p < dif + 100 && input[p] != ')')	++p;
	if (input[p] != ')') {
		printf("function ')'wrong format!\n");
		return ERROR;
	}
	x1 = expression(input, pre_pos, p);
	printf("%lf", x1);
	return x1;
}

/*help information*/
double help_(char* input, int p) {
	printf("--------------------------help information------------------------\n");
	printf(" if, while, and function define supported;\n you can also type in a expression to calculate or a statement to judge;\n\n");
	printf("examples like this:\n(1+3*4/(8/2))+1.9\nx=3;y=4;(1+x*y/(8/2))+1.9;\n if(x>y){ x=x+1;y=y-1;};\n x=1;sum=0;while(x<=100){sum=sum+x;x=x+1;};sum;\n");
	printf("\nyou can also define you own functions, in the form:\n define(function name){ function body };\n");
	printf("for example:\n define(ret){if(x<=2){return 1;}; x=x-1; y=ret(x); x=x-1; return y+ret(x);}; x=5; ret(x);\n");
	printf("\ndefining array is also supported: defarray(arrayname,maxsize); \nyou can call an array unit by arrayname[index];\n");
	printf("examples like this:\ndefarray(a,10);a;i=0;while(i<10){a[i]=i;i=i+1;};a;\n");
	printf("\nyou can also define string and opearate it as arrays:\n");
	printf("examples like this: a=\"string\";a;a[5]\n");
	printf("\nsystem functions:\n mathfunctions: sin(x);cos(x);sqrt(x);pow(x,y);abs(x);\n other functions:\n ");
	printf("getnum(),get a number from input; exit() or quit() to quit(); brainfuck() to open the brainfuck mode;\n");
	printf("printnum(x) to print the num to screen;\n");
	printf("data types: double, string, array;\n can be init by x=1.23 / x=\"string\" / define(a,10);");
	return 0;
};

/*open the brainfuck interpreter;*/
double brainfuck_(char* input, int p)
{
	printf("brainfuck mode: have fun!\n");
	printf("( enter exit to quit )\n");
	printf("hello world example: ++++++++++[>+>+++>+++++++>++++++++++<<<<-]>>>++.>+.+++++++..+++.<<++.>+++++++++++++++.>.+++.------.--------.<<+.<.\n");
	char f[5000];
	while (1) {
		printf("\n>>");
		gets(f);
		if (strstr(f, "exit") != NULL) // exit by enter "exit";
			return 0;
		else
			interpret(f);
	}
	return 0;
}
/*use to fedine an array: defarray(arrayname,maxsize);*/
double defarray_(char* input, int p) {
	int dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p;
	/*get the array name*/
	char name[30];
	int pre_pos ,name_pos=0;
	while (isalpha(input[p]))	name[name_pos++] = input[p++];
	name[name_pos] = 0;

	/*get the maxsize of array*/
	while (input[p] && p < dif + 100 && input[p] != ',')	++p;
	if (input[p] != ',') {
		printf("function ',' wrong format!\n");
		return ERROR;
	}
	++p;
	pre_pos = p;
	while (input[p] && p < dif + 200 && input[p] != ')')	++p;
	if (input[p] != ')') {
		printf("function ')'wrong format!\n");
		return ERROR;
	}
	int max_size = expression(input, pre_pos, p);
	if (max_size <= 0) {
		printf("range must be positive!\n");
		return ERROR;
	}
	ElementType Elem;
	Elem.type = array;
	Elem.f = (double*)malloc(sizeof(double)*max_size);
	for (int i = 0; i < max_size; ++i) {
		((double*)Elem.f)[i] = 0;
	}
	Elem.value1 = 0;
	Elem.value2 = max_size;
	L = Insert_list(L, Elem);
	Insert(name, &L->Data, T->Data);
	return max_size;
}

/* define(function name){ function body }; */
double fun_(char* input, int p, ElementType *pele) {
	double x = 0;
	HashTable H;
	H = InitializeTable(30);//init a new name space;
	T = Insert_tables(T, H);//insert the name space in the stack
	char var_name[30] = { 0 };
	int vn_pos = 0, dif = p;
	while (input[p] && p < dif + 100 && isspace(input[p])) ++p; //remove the space
	
	/*get the function parameters*/
	while (input[p] && p < dif + 300 &&input[p] != ')') {
		if (isalpha(input[p])) {
			var_name[vn_pos++] = input[p]; //get the name;
		}
		else { //init this parameter
			var_name[vn_pos] = 0;
			vn_pos = 0;
			ElementType *pele1 = NULL;
			if ((pele1 = find_Elem(var_name)) != NULL) {
				ElementType Elem;
				Elem.type = pele1->type;
				Elem.value1 = pele1->value1;
				Elem.value2 = pele1->value2;
				Elem.f = pele1->f;
				L = Insert_list(L, Elem);
				Insert(var_name, &L->Data, T->Data);
			}
			else {
				printf("the var pass to function is not init!\n");
				return ERROR;
			}
		}
		p++;
	}
	if (input[p] != ')') {
		printf("function ')'wrong format!\n");
		return ERROR;
	}
	//init the last parameter;
	var_name[vn_pos] = 0;
	vn_pos = 0;
	ElementType *pele1 = NULL;
	if ((pele1 = find_Elem(var_name)) != NULL) {
		ElementType Elem;
		Elem.type = pele1->type;
		Elem.value1 = pele1->value1;
		Elem.value2 = pele1->value2;
		Elem.f = pele1->f;
		L = Insert_list(L, Elem);
		Insert(var_name, &L->Data, T->Data);
	}
	++p;

	/*process the function block*/
	int pos = pele->value1 + 1, before;
	double stat = PASS;
	while (pos < pele->value2) {
		char a_st[100] = { 0 };
		int a_pos = 0;
		before = pos;
		while (pos < pele->value2 && isspace(functions[pos]))	++pos;
		//get the sentence in the function
		while (pos < pele->value2 && functions[pos] != '{' && functions[pos] != ';')	a_st[a_pos++] = functions[pos++];
		if (functions[pos] == '{') {
			charstack s = newcharstack();
			pushchar(s, '{');
			int flag1 = 0;
			while (pos < pele->value2 && !csempty(s)) {
				a_st[a_pos++] = functions[pos];
				if (functions[pos] == '{'&&flag1)	pushchar(s, '{');
				else if (functions[pos] == '}')	popchar(s);
				pos++;
				flag1 = 1;
			}
		}
		while (pos < pele->value2 && functions[pos] != ';')	a_st[a_pos++] = functions[pos++];
		a_st[a_pos] = 0;
		//if the sentence include return sentecnes;
		if ((strstr(a_st, "return")) != NULL) {
			if (strstr(a_st, "return") == a_st){//if the sentence is a return sentence;
				a_pos = strstr(a_st, "return") - a_st;
				x=expression(a_st, a_pos + 6, strlen(a_st));
				T=RmFirst_tables(T);
				return x;
			}
			else {//if the return sentence is in a block;
				stat = sentence(functions, before, pos);
				if (stat != PASS && stat!=ERROR ) {
					T = RmFirst_tables(T);
					printf("return num = %lf\n", stat);
					return stat;
				}
			}
		}else
			stat = sentence(functions, before, pos);
		if (functions[pos] == ';') ++pos;
	}
	T=RmFirst_tables(T);
	return x;
}

//用来在变量作用域栈中寻找变量；
ElementType *find_Elem(char* name) {
	Tables T1=T;
	ElementType *pele=NULL;
	while (T1 != NULL) {
		pele=Retrieve(Find(name, T1->Data), T1->Data);
		if (pele != NULL)
			return pele;
		else
			T1 = T1->Next;
	}
	return pele;
}

//初始化变量作用域和系统函数；
void ini_functions() {
	ElementType Elem;

	HashTable H;
	H = InitializeTable(50);
	T = Insert_tables(T, H);

	Elem.type = function;
	Elem.f = &pow_;
	Elem.value1 = Elem.value2=0;
	L = Insert_list(L, Elem);
	Insert("pow", &L->Data, H);

	Elem.type = function;
	Elem.f = &abs_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("abs", &L->Data, H);

	Elem.type = function;
	Elem.f = &sin_;
	Elem.value1 = Elem.value2 = 0;;
	L = Insert_list(L, Elem);
	Insert("sin", &L->Data, H);

	Elem.type = function;
	Elem.f = &cos_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("cos", &L->Data, H);

	Elem.type = function;
	Elem.f = &tan_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("tan", &L->Data, H);

	Elem.type = function;
	Elem.f = &sqrt_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("sqrt", &L->Data, H);

	Elem.type = function;
	Elem.f = &exit_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("exit", &L->Data, H);

	Elem.type = function;
	Elem.f = &exit_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("quit", &L->Data, H);

	Elem.type = function;
	Elem.f = &help_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("help", &L->Data, H);

	Elem.type = function;
	Elem.f = &brainfuck_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("brainfuck", &L->Data, H);

	Elem.type = function;
	Elem.f = &getnum_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("getnum", &L->Data, H);

	Elem.type = function;
	Elem.f = &printnum_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("printnum", &L->Data, H);

	Elem.type = function;
	Elem.f = &defarray_;
	Elem.value1 = Elem.value2 = 0;
	L = Insert_list(L, Elem);
	Insert("defarray", &L->Data, H);

}