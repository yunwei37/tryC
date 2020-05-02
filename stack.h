/*用于表达式计算的栈的声明*/
#ifndef _stack_h
#define _stack_h

typedef struct _charnode /*字符结点*/ 
{
	char a;
	struct _charnode *next;
} *charstack;

typedef struct _doublenode /*浮点数结点*/
{
	double a;
	struct _doublenode *next;
} *doublestack;


charstack newcharstack();
 /*创建字符栈并初始化*/ 
 
doublestack newdoublestack();
 /*创建浮点数栈并初始化*/
 
int csempty(charstack s);
 /*判断是否空栈*/
int dsempty(doublestack s);

void pushchar(charstack s,char a);
 /*压入字符*/
 
void popchar(charstack s);
/*弹出字符*/ 

void pushdouble(doublestack s,double a);
 /*压入数字*/
 
void popdouble(doublestack s);
 /*弹出数字*/ 
 
char cstop(charstack s);
 /*返回顶端字符*/
 
double dstop(doublestack s);
/*返回顶端数字*/

#endif
