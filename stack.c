#include <stdio.h>
#include <stdlib.h>
#include "stack.h" 

charstack newcharstack() /*创建字符栈并初始化*/ 
{
	charstack s;
	s=malloc(sizeof(struct _charnode));
	s->next=NULL;
	return s;
}

doublestack newdoublestack() /*创建浮点数栈并初始化*/
{
	doublestack s;
	s=malloc(sizeof(struct _doublenode));
	s->next=NULL;
	return s;
}

int csempty(charstack s) /*判断是否空栈*/
{
	if(s->next==NULL)
		return 1;
	else return 0;
	
}
int dsempty(doublestack s)
{
	if(s->next==NULL)
		return 1;
	else return 0;
}

void pushchar(charstack s,char a) /*压入字符*/
{
	charstack newnode;
	newnode=malloc(sizeof(struct _charnode));
	newnode->a=a;
	newnode->next=s->next;
	s->next=newnode;
}

void popchar(charstack s) /*弹出字符*/ 
{
	charstack newnode;
	if(csempty(s))
		perror("the stack's empty");
	else
	{
		newnode=s->next;
		s->next=s->next->next;
		free(newnode);
	}
}

void pushdouble(doublestack s,double a) /*压入数字*/
{
	doublestack newnode;
	newnode=malloc(sizeof(struct _doublenode));
	newnode->a=a;
	newnode->next=s->next;
	s->next=newnode;
}

void popdouble(doublestack s) /*弹出数字*/ 
{
	doublestack newnode;
	if(dsempty(s))
		perror("the stack's empty");
	else
	{
		newnode=s->next;
		s->next=s->next->next;
		free(newnode);
		
	}
}

char cstop(charstack s)/*返回顶端字符*/
{
	if(!csempty(s))
		return s->next->a;
	perror("the stack's empty");
	return 0;
}

double dstop(doublestack s)/*返回顶端数字*/
{
	if(!dsempty(s))
		return s->next->a;
	perror("the stack's empty");
	return 0;
}
