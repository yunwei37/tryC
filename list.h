/* list.h: the impliment of a list without headnode.
 * version 1.0 by ZhengYusheng 25/3/2018
 */

#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "quadHash.h"

/* the defination of the list structure*/
//变量保存链表的结构声明
typedef struct LNode *PtrToLNode;
struct LNode {
    ElementType Data;
    PtrToLNode Next;
};
typedef PtrToLNode List;
//变量作用域链表的结构声明；
typedef struct HashNode *PtrToHNode;
struct HashNode {
	HashTable Data;
	PtrToHNode Next;
};
typedef PtrToHNode Tables;

List L;//在运行过程中自动分配的哈希表变量单元会被实际存放在这个链表里面

Tables T;//由哈希表组成的栈（链表实现），模拟在函数运行过程中的变量作用域栈

/* Insert X before the node pointed to by location P, 
 * and return the list header of the linked list. 
 * If parameter P points to an illegal location, 
 * print "Wrong Position for Insertion" and return ERROR.
 */ 
List Insert_list( List L, ElementType X );//用来插入新的变量节点；

/* Delete the element of position P and return to the 
 * list header of the linked list. If parameter P points to an illegal location, 
 * print "Wrong Position for Deletion" and return ERROR.
 * List Delete( List L, Position P );
 */
List Delete_list( List L, ElementType * X );//用来删除被更新的变量节点；

Tables Insert_tables(Tables L, HashTable H);//在创建函数的时候进行变量作用域表的插入；

Tables RmFirst_tables(Tables T);//在函数结束时弹出变量作用域表；



#endif
