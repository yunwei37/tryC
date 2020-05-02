
/*
 * actualize tiny interpreter
 * version 1.0
 *  2019.3.28
 * -------------
 * support decimal,sign,multiple brackets 
 * no blank is allowed between number and operator 
 * support sin/cos/tan/abs/pow/sqrt functions
 * support if statement, while statement, and defining varities and functions
 * 
 */
 
//main 方法也在calculator.c里面
#ifndef CALCULATOR_H_
#define CALCULATOR_H_

//for vs,because no s_scanf();
#define _CRT_SECURE_NO_WARNINGS

#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "quadHash.h"
#include "list.h" 

//语句块和语句的两种状态定义；
#define ERROR 1e9//出错，将错误状态上抛
#define PASS 1e-9//正常运行

double ans; //存放每一次计算以后的结果可供直接使用；

/*functions buffer*/
char functions[2000];//自定义函数的存放空间；
int function_pos;//自定义函数已使用空间的指针；

Tables T;/*name space*/

/*For input expressions character-by-character processing and generating infix expressions, the number position remains unchanged,while the operator position is changed*/ 
double expression(char* input, int start, int end);//对于表达式的计算

//处理一个语句；返回值用来判断语句状态，以及如果其中存在函数的return，使用其返回；
double sentence(char* input, int start, int end);

//用来处理布尔值判断，true返回1，false返回0；ERROR返回代表不是布尔值；
int bool(char* input, int start, int end);

//处理一个语句块：返回值用来判断语句状态，以及如果其中存在函数的return，使用其返回；
double block(char* input, int start, int end);

extern void ini_functions();//用来初始化主程序的函数和命名空间；

#endif
