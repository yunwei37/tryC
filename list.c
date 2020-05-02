#include "list.h"

List Insert_list(List L, ElementType X) {
	List a;
	a = (List)malloc(sizeof(struct LNode));
	a->Data = X;
	a->Next = L;
	return a;//insert at the head node;
}

List Delete_list(List L, ElementType *X) {
	if (X == NULL)	return L;//解决意外...
	List pre = L, a;
	if (&L->Data == X) {
		a = L;
		L = L->Next;
		free(a);
		return L;//delete the head node;
	}
	while ( pre->Next &&(&(pre->Next->Data)!=X))
		pre = pre->Next;// find the pre node of deletion;
	a = pre->Next;
	pre->Next = pre->Next->Next;
	free(a);
	return L;//do the deetion;
}

Tables Insert_tables(Tables T, HashTable H) {
	Tables a;
	a = (Tables)malloc(sizeof(struct HashNode));
	a->Data = H;
	a->Next = T;
	return a;//insert at the head node;
}

Tables RmFirst_tables(Tables T ){
	Tables a=T;
	DestroyTable(T->Data);
	T = T->Next;
	free(a);
	return T;//delete the head node;
}


