#include "quadHash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#define MinTableSize (10)

/*Nextprime和hash为两个私有函数。 */
/* Return next prime; assume N >= 10 */
static int NextPrime(int N)
{
	int i;
	if (N % 2 == 0)
		N++;
	for (; ; N += 2)
	{
		for (i = 3; i * i <= N; i += 2)
			if (N % i == 0)
				goto ContOuter;  /* Sorry about this! */
		return N;
	ContOuter:;
	}
}

/*
Index Hash(KeyType Key, Index TableSize)// Hash function for ints
{
	return Key % TableSize;
}
*/

Index Hash(KeyType Key, Index TableSize)
{
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc..*/
    unsigned int hash = 0;
    while (*Key)
    {
        hash = hash * seed + (*Key++);
    }
    return (hash & 0x7FFFFFFF)%TableSize;
}

HashTable InitializeTable(Index TableSize)/*创建一个hash表并返回。 */
{
	HashTable H;
	int i;
	if (TableSize < MinTableSize)
	{
		printf("Table size too small");
		return NULL;
	}
	/* Allocate table */
	H = malloc(sizeof(struct HashTbl));
	if (H == NULL) {
		printf("Out of space!!!");
		return H;
	}
	H->TableSize = NextPrime(TableSize);
	H->num = 0;
	/* Allocate array of Cells */
	H->TheCells = malloc(sizeof(Cell) * H->TableSize);
	if (H->TheCells == NULL) {
		printf("Out of space!!!");
		return H;
	}
	for (i = 0; i < H->TableSize; i++) {
		H->TheCells[i].key[0]=0;
		H->TheCells[i].pElement = NULL;
		H->TheCells[i].Info = Empty;
	}
	return H;
}

Position Find(KeyType Key, HashTable H)/*输入关键字和hash表，返回在hash表中的索引，元素情况可查看.info */
{
	Position CurrentPos;
	int CollisionNum;
	CollisionNum = 0;
	CurrentPos = Hash(Key, H->TableSize);
	while (H->TheCells[CurrentPos].Info != Empty &&
		strcmp(H->TheCells[CurrentPos].key , Key)!=0)/* Probably need strcmp!! */
	{
		CurrentPos += 2 * ++CollisionNum - 1;
		if(CurrentPos >= H->TableSize)
			CurrentPos -= H->TableSize;
		if (CurrentPos >= H->TableSize * 2) Rehash(H);
	}
	return CurrentPos;
}


int Insert(KeyType Key, ElementType * pElem, HashTable H)/*输入hash值和元素指针以及hash表，将元素插入其中。 */
{
	Position Pos;
	Pos = Find(Key, H);
	if (H->TheCells[Pos].Info != Legitimate)/* OK to insert here */
	{
		H->TheCells[Pos].Info = Legitimate;
		strcpy(H->TheCells[Pos].key , Key);
		H->TheCells[Pos].pElement = pElem;
		/* Probably need strcpy! */
		H->num++;
		return 1;
	}
	else if(H->TheCells[Pos].pElement->type != pElem->type ||H->TheCells[Pos].pElement->value1!=pElem->value1 || H->TheCells[Pos].pElement->value2 != pElem->value2){
		Delete_list(L, H->TheCells[Pos].pElement);
		H->TheCells[Pos].pElement = pElem;
		H->num++;
		return 1;
	}
	return 0;
}

HashTable Rehash(HashTable H)/*将旧表扩大为2倍以上，返回新表。 */
{
	int i, OldSize,OldNum;
	Cell *OldCells;
	OldCells = H->TheCells;
	OldSize = H->TableSize;
	OldNum = H->num;
	/* Get a new, empty table */
	H = InitializeTable(2 * OldSize);

	/* Scan through old table, reinserting into new */
	for (i = 0; i < OldSize; i++)
		if (OldCells[i].Info == Legitimate)
			Insert(OldCells[i].key, OldCells[i].pElement, H);

	free(OldCells); 
	return H;
}

ElementType* Retrieve(Position P, HashTable H)/*获取hash表某个位置的元素的指针。 */
{
	return H->TheCells[P].pElement;
}

void DestroyTable(HashTable H)
{
	int i;
	for (i = 0; i < H->TableSize; i++) {
		L = Delete_list(L, H->TheCells->pElement);
	}
	free(H->TheCells);
	free(H);
}
