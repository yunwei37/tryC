/*deal with rhe functions and namespaces*/
/*这个文件里面主要是系统自带的一些被定义成指针的函数，以及自定义函数的实现和变量函数作用域的初始化*/
#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "calculator.h"
#include "quadHash.h"
#include "list.h"

//自定义函数的实现过程，使用c语言函数模拟；
double fun_(char* input, int p,ElementType *pele);

//初始化变量作用域和系统函数；
void ini_functions();

//用来在变量作用域栈中寻找变量；
ElementType *find_Elem(char* name);

#endif
