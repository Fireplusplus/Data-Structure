#ifndef __SEQLIST_H__
#define __SEQLIST_H__


#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MAX_SIZE 10


typedef int DataType;
//typedef unsigned int size_t;

typedef struct SeqList
{
	DataType array[MAX_SIZE];
	size_t size;
}SeqList, *PSeqList;


//初始化顺序表
void InitSeqList(PSeqList pSeqList);

//尾插
void PushBack(PSeqList pSeqList, DataType data);

//尾删
void PopBack(PSeqList pSeqList);

//打印顺序表
void PrintSeqList(PSeqList pSeqList);

//头插
void PushFront(PSeqList pSeqList, DataType data);

//头删
void PopFront(PSeqList pSeqList);

//任意位置插入
void Insert(PSeqList pSeqList, int pos, DataType data);

//在顺序表中查找元素data
int Find(PSeqList pSeqList, DataType data);

//删除顺序表中pos位置上的元素
void Erase(PSeqList pSeqList, int pos);

//移除顺序表中的元素data
void Remove(PSeqList pSeqList, DataType data);

//移除顺序表中的所有元素data
void RemoveAll(PSeqList pSeqList, DataType);


#endif
