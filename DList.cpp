#include <iostream>

using namespace std;

typedef int DataType;

class Node
{
public:
	Node(const DataType &val = 0)
		: data(val)
		, pNext(NULL)
		, pPre(NULL)
	{}

	DataType data;
	Node *pNext;
	Node *pPre;
};

class DList
{
public:
	DList()
		: pHead(NULL)
		, pTail(NULL)
		, size(0)
	{}

	void PushBack(const DataType &data)
	{
		Node *NewNode = new Node(data);
		
		if (NULL == pHead)		//链表为空
			pHead = NewNode;
		else				//不为空
			pTail->pNext = NewNode;
		NewNode->pPre = pTail;
		pTail = NewNode;
		size++;
	}

	void PopBack()
	{
		if (NULL == pHead)		//链表为空
			return;

		if (1 == size)			//只有一个结点
		{
			delete pHead;
			pHead = pTail = NULL;
		}
		else				//有多个结点
		{
			pTail = pTail->pPre;
			delete pTail->pNext;
			pTail->pNext = NULL;
		}
		size--;
	}
	
	void Insert(Node *pos, const DataType &data)
	{
		if (NULL == pos)
			return;
		
		//检测pos所指结点是否在此链表中
		Node *pNode = pHead;
		while (NULL != pNode && pNode != pos)
			pNode = pNode->pNext;
		if (NULL == pNode)
			return;

		Node *NewNode = new Node(data);
		NewNode->pPre = pos;
		if (pos != pTail)
		{
			NewNode->pNext = pos->pNext;
			pos->pNext->pPre = NewNode;
		}
		else
		{
			pTail = NewNode;
		}
		pos->pNext = NewNode;
		size++;
	}

	void PushFront(const DataType &data)
	{
		Node *NewNode = new Node(data);
		
		if (NULL == pHead)
		{
			pTail = NewNode;
		}
		else
		{
			NewNode->pNext = pHead;
			pHead->pPre = NewNode;
		}
		pHead = NewNode;
		size++;	
	}

	void PopFront()
	{
		if (0 == size)
			return;
		
		if (1 == size)
		{
			delete pHead;
			pHead = pTail = NULL;
		}
		else
		{
			Node *NodeToDel = pHead;
			pHead = pHead->pNext;
			pHead->pPre = NULL;
			delete NodeToDel;
		}
		size--;
	}
	
	Node *Find(const DataType &data)
	{
		Node *pNode = pHead;

		while (NULL != pNode && pNode->data != data)
			pNode = pNode->pNext;
		
		return pNode;	
	}
	
	friend ostream & operator<<(ostream &os, DList &list)
	{
		Node *pNode = list.pHead;

		while (NULL != pNode)
		{
			os<<pNode->data<<"->";
			pNode = pNode->pNext;
		}
		os<<"NULL";

		return os;
	}
	
	~DList()
	{
		Clear();
	}

private:

	void Clear()
	{
		Node *pNode;

		while (NULL != pHead)
		{
			pNode = pHead;
			pHead = pHead->pNext;
			delete pNode;
		}
		pTail = NULL;
		size = 0;
	}

private:
	Node *pHead;
	Node *pTail;
	int size;
};

void test()
{
	DList d1;

	d1.PushBack(1);
	d1.PushBack(2);
	d1.PushBack(3);
	d1.PushBack(4);
	d1.PushBack(5);

	cout<<d1<<endl;

	d1.PopBack();
	cout<<d1<<endl;
	d1.PopBack();
	d1.PopBack();
	cout<<d1<<endl;
	d1.PopBack();
	d1.PopBack();
	d1.PopBack();

	cout<<d1<<endl;
}

void test2()
{
	DList d1;

	d1.PushFront(1);
	d1.PushFront(2);
	d1.PushFront(3);
	d1.PushFront(4);
	d1.PushFront(5);
	d1.PushFront(6);

	cout<<d1<<endl;

/*	d1.PopFront();
	cout<<d1<<endl;
	d1.PopFront();
	d1.PopFront();
	d1.PopFront();
	d1.PopFront();
	d1.PopFront();
	d1.PopFront();

	cout<<d1<<endl;
*/
	Node *pNode = d1.Find(4);
	d1.Insert(pNode, 10);
	cout<<d1<<endl;
	pNode = d1.Find(6);
	d1.Insert(pNode, 11);
	cout<<d1<<endl;
}

int main()
{
	//test();
	test2();

	return 0;
}
