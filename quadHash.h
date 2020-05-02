/* 平方散列型hash函数，每个哈希表就是一个变量作用域*/

#ifndef _HashQuad_H
#define _HashQuad_H

#define _CRT_SECURE_NO_WARNINGS

	enum KindOfEntry { Legitimate, Empty, Deleted };//哈希表单元状态标识；
	enum datatype { var , function , boolean, self, string, array };//元素类型标识；

	struct hashElem;
	typedef struct hashElem ElementType;//哈希表的入口，用来存储指向每个元素的指针；元素在运行时自动分配内存空间
	
	struct HashEntry;
	typedef struct HashEntry Cell;/*条目定义为cell ,这个不是指针*/ 

	struct HashTbl;
	typedef struct HashTbl *HashTable;//哈希表

	typedef char* KeyType;//用来在哈希表里面存储变量名
	
	typedef double (*afunc)(char* input, int p); /*内置函数*/

	typedef double(*self_fun)(char* input, int p,ElementType* pele);//自己定义的函数

	struct hashElem { //自定义元素结构，用来存储变量，总共有
		enum datatype type;//元素类型;
		double value1;//双精度浮点型，默认元素类型，在自定义函数中用来标识函数块开头
		int value2;//整型变量和布尔值，在自定义函数中用来标识函数块结尾
		void* f;//指针：可以是函数指针，也可以是数组和字符串指针
	}; 

	struct HashEntry//每个哈希表单元的入口
	{
		char key[30];
		ElementType*  pElement;
		enum KindOfEntry Info;
	};

	struct HashTbl//哈希表主体结构
	{
		int TableSize;
		int num;
		Cell *TheCells;    /* Cell *TheCells will be an array of  HashEntry cells, allocated later*/
	};

	typedef unsigned int Index;
	typedef Index Position;

	HashTable InitializeTable(Index TableSize);/*创建一个hash表并返回。 */
	void DestroyTable(HashTable H); /*干掉这个哈希表 */
	Position Find(KeyType Key, HashTable H);/*输入关键字和hash表，返回在hash表中的索引，元素情况可查看.info */
	int Insert(KeyType Key, ElementType* pElem, HashTable H);/*输入hash值和元素指针以及hash表，将元素插入其中。 */
	ElementType* Retrieve(Position P, HashTable H);/*获取hash表某个位置的元素的指针。 */
	HashTable Rehash(HashTable H);/*将旧表扩大为2倍以上，返回新表。 */


#endif  /* _HashQuad_H */
