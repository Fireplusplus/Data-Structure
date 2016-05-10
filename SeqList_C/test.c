#include "SeqList.h"

void test()
{
	SeqList seq;
	InitSeqList(&seq);
	PushBack(&seq, 1);
	PushBack(&seq, 2);
	PushBack(&seq, 3);
	PushBack(&seq, 4);

	PrintSeqList(&seq);

	PopBack(&seq);
	PrintSeqList(&seq);
	PopBack(&seq);
	PrintSeqList(&seq);
	PopBack(&seq);
	PrintSeqList(&seq);
	PopBack(&seq);
	PrintSeqList(&seq);

}

void test2()
{
	
	SeqList seq;
	InitSeqList(&seq);
	PushFront(&seq, 1);
	PushFront(&seq, 2);
	PushFront(&seq, 3);
	PushFront(&seq, 4);

	PrintSeqList(&seq);

	PopFront(&seq);
	PrintSeqList(&seq);
	PopFront(&seq);
	PrintSeqList(&seq);
	PopFront(&seq);
	PrintSeqList(&seq);
	PopFront(&seq);

}

void test3()
{
	
	SeqList seq;
	InitSeqList(&seq);
	PushFront(&seq, 1);
	PushFront(&seq, 2);
	PushFront(&seq, 3);
	PushFront(&seq, 4);

	PrintSeqList(&seq);

	Remove(&seq, 1);
	PrintSeqList(&seq);
	Remove(&seq, 3);
	PrintSeqList(&seq);
	Remove(&seq, 4);
	PrintSeqList(&seq);
	Remove(&seq, 10);
	PrintSeqList(&seq);

}

void test4()
{
	
	SeqList seq;
	InitSeqList(&seq);
	PushFront(&seq, 1);
	PushFront(&seq, 2);
	PushFront(&seq, 1);
	PushFront(&seq, 1);

	PrintSeqList(&seq);

	RemoveAll(&seq, 1);
	PrintSeqList(&seq);

}

int main()
{ 
	//test();
	//test2();
	//test3();
	test4();
	
	return 0;
}
