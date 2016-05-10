#include "SeqList.h"



void InitSeqList(PSeqList pSeqList)
{
	assert(NULL != pSeqList);
	
	memset(pSeqList->array, 0, MAX_SIZE * sizeof(DataType));
	pSeqList->size = 0;	
}

void PushBack(PSeqList pSeqList, DataType data)
{
	Insert(pSeqList, pSeqList->size, data);
	
	/*assert(NULL != pSeqList);

	if (MAX_SIZE >= pSeqList->size)
	{
		return;
	}
	pSeqList->array[pSeqList->size] = data;
	pSeqList->size++;*/
}

void PopBack(PSeqList pSeqList)
{
	Erase(pSeqList, pSeqList->size-1);

	/*assert(NULL != pSeqList);
	
	if (0 >= pSeqList->size)
	{
		return;
	}

	pSeqList->size--;*/
}

void PrintSeqList(PSeqList pSeqList)
{
	int i = 0;

	assert(NULL != pSeqList);

	for (i = 0; i < pSeqList->size; i++)
	{
		printf("%d ", pSeqList->array[i]);
	}
	printf("\n");
}

void PushFront(PSeqList pSeqList, DataType data)
{
	Insert(pSeqList, 0, data);
	
	/*int i = 0;

	assert(NULL != pSeqList);

	if (MAX_SIZE <= pSeqList->size)
	{
		return;
	}

	for (i = pSeqList->size; i > 0; i++)
	{
		pSeqList->array[i] = pSeqList->array[i - 1];
	}
	pSeqList->array[0] = data;
	pSeqList->size++;*/
}

void PopFront(PSeqList pSeqList)
{
	Erase(pSeqList, 0);
	
	/*int i = 0;
	
	assert(NULL != pSeqList);

	if (0 >= pSeqList->size)
	{
		return;
	}

	for (i = 0; i < pSeqList->size - 1; i++)
	{
		pSeqList->array[i] = pSeqList->array[i + 1];
	}*/
}

void Insert(PSeqList pSeqList, int pos, DataType data)
{
	int i = 0;

	assert(NULL != pSeqList && (0 <= pos && pSeqList->size >= pos));

	if (MAX_SIZE <= pSeqList->size)
		return;

	for (i = pSeqList->size; i > pos; i--)
	{
		pSeqList->array[i] = pSeqList->array[i - 1];
	}
	pSeqList->array[pos] = data;
	pSeqList->size++;
}

int Find(PSeqList pSeqList, DataType data)
{
	int i = 0;

	assert(NULL != pSeqList);

	for (i = 0; i < pSeqList->size; i++)
	{
		if (data == pSeqList->array[i])
			return i;
	}

	return -1;
}

void Erase(PSeqList pSeqList, int pos)
{
	int i = 0;

	assert(NULL != pSeqList && (0 <= pos && pSeqList->size > pos));
	
	for (i = pos; i < pSeqList->size - 1; i++)
	{
		pSeqList->array[i] = pSeqList->array[i + 1];
	}
	pSeqList->size--;
}

void Remove(PSeqList pSeqList, DataType data)
{
	Erase(pSeqList, Find(pSeqList, data));
}


void RemoveAll(PSeqList pSeqList, DataType data)
{
	int left = -1, right = 0;
	int len = pSeqList->size;

	assert(NULL != pSeqList);
	
	for (right; right < len; right++)
	{
		if (data != pSeqList->array[right])
		{
			left++;
			pSeqList->array[left] = pSeqList->array[right];	
			pSeqList->size++;
		}
			pSeqList->size--;
	}
}
