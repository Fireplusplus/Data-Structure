#include "SeqList.h"

static int CheckCapacity(PSeqList pSeqList)
{
	assert(NULL != pSeqList);

	if (pSeqList->size >= pSeqList->capacity)
	{
		PDataType tmp = (PDataType)realloc(pSeqList->pData, 
		(pSeqList->capacity + DEFAULT_INCREASE_SIZE) * sizeof(DataType));
		if (NULL == tmp)
		{
			perror("pSeqList->pData realloc failured");
			return -1;
		}
		pSeqList->pData = tmp;
		pSeqList->capacity += DEFAULT_INCREASE_SIZE;
	}

	return 0;
}

void InitSeqList(PSeqList pSeqList)
{
	assert(NULL != pSeqList);
	
	pSeqList->pData = (PDataType)malloc(DEFAULT_SIZE * sizeof(DataType));
	if (NULL == pSeqList->pData)
	{
		perror("pSeqList->data malloc failured");
		return;
	}
	memset(pSeqList->pData, 0, DEFAULT_SIZE * sizeof(DataType));
	pSeqList->size = 0;
	pSeqList->capacity = DEFAULT_SIZE;
}

void destroy(PSeqList pSeqList)
{
	assert(pSeqList);

	free(pSeqList->pData);
	pSeqList->pData = NULL;
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

	assert(NULL != pSeqList && NULL != pSeqList->pData);

	for (i = 0; i < pSeqList->size; i++)
	{
		printf("%d ", pSeqList->pData[i]);
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

	assert(NULL != pSeqList && NULL != pSeqList->pData && (0 <= pos && pSeqList->size >= pos));

	if (0 != CheckCapacity(pSeqList))
		return;

	for (i = pSeqList->size; i > pos; i--)
	{
		pSeqList->pData[i] = pSeqList->pData[i - 1];
	}
	pSeqList->pData[pos] = data;
	pSeqList->size++;
}

int Find(PSeqList pSeqList, DataType data)
{
	int i = 0;

	assert(NULL != pSeqList && NULL != pSeqList->pData);

	for (i = 0; i < pSeqList->size; i++)
	{
		if (data == pSeqList->pData[i])
			return i;
	}

	return -1;
}

void Erase(PSeqList pSeqList, int pos)
{
	int i = 0;

	assert(NULL != pSeqList && NULL != pSeqList->pData && (0 <= pos && pSeqList->size > pos));
	
	for (i = pos; i < pSeqList->size - 1; i++)
	{
		pSeqList->pData[i] = pSeqList->pData[i + 1];
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

	assert(NULL != pSeqList && NULL != pSeqList->pData);
	
	for (right; right < len; right++)
	{
		if (data != pSeqList->pData[right])
		{
			left++;
			pSeqList->pData[left] = pSeqList->pData[right];	
			pSeqList->size++;
		}
			pSeqList->size--;
	}
}
