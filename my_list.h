#pragma once

#include <iostream>

using namespace std;

template <typename T>
struct __ListNode
{
	T _val;
	__ListNode* _next;
	__ListNode* _prev;

	__ListNode(const T& val)
		: _val(val)
		, _next(NULL)
		, _prev(NULL)
	{}
};

template <typename T, typename Ref, typename Ptr> 
class __ListIterator
{
	typedef __ListIterator<T, Ref, Ptr> Self;
public:
	__ListIterator()
	{}

	__ListIterator(__ListNode<T>* node)
		: _node(node)
	{}

	Ref operator*()
	{
		return _node->_val;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		_node = _node->_next;
		return *this;
	}

	Self operator++(int)
	{
		Self tmp(*this);
		_node = _node->_next;
		return tmp;
	}

	bool operator==(const Self& it) const
	{
		return _node == it._node;
	}

	bool operator!=(const Self& it) const
	{
		return !(*this == it);
	}

public:
	__ListNode<T>* _node;
};


template <typename T>
struct List
{
	typedef __ListNode<T> Node;
	typedef __ListIterator<T, T&, T*> Iterator;
	typedef __ListIterator<T, const T&, const T*> ConstIterator;
public:
	List()
		: _head(BuyNode(T()))
	{
		_head->_next = _head;
		_head->_prev = _head;
	}

	//void PushBack(const T& val)
	//{
	//	Node* tmp = BuyNode(val);
	//	Node *tail = _head->_prev;

	//	tmp->_next = _head;
	//	tmp->_prev = tail;
	//	tail->_next = tmp;
	//	_head->_prev = tmp;
	//}

	void PushBack(const T& val)
	{
		Insert(End(), val);
	}

	void PushFront(const T& val)
	{
		Insert(Begin(), val);
	}

	void PopBack()
	{
		Erase(--End());
	}

	void PopFront()
	{
		Erase(Begin());
	}

	void Insert(const Iterator pos, const T& val)
	{
		Node* next = pos._node;
		Node* prev = next->_prev;

		Node* tmp = new Node(val);
		tmp->_next = next;
		next->_prev = tmp;
		tmp->_prev = prev;
		prev->_next = tmp;
	}

	void Erase(const Iterator pos)
	{
		Node* next = (pos._node)->_next;
		Node* prev = (pos._node)->_prev;
		prev->_next = next;
		next->_prev = prev;

		delete pos._node;
	}
	
	Iterator Begin()
	{
		return Iterator(_head->_next);
	}

	Iterator End()
	{
		return Iterator(_head);
	}

	ConstIterator Begin() const
	{
		return Iterator(_head->_next);
	}

	ConstIterator End() const
	{
		return Iterator(_head);
	}
protected:
	Node* BuyNode(const T& val)
	{
		Node* tmp = new Node(val);
		return tmp;
	}
protected:
	Node* _head;
};

void TestList()
{
	List<int> l;
	for (int i = 0; i < 10; i++)
		l.PushBack(i);

	List<int>::Iterator pos = l.Begin();
	while (pos != l.End())
	{
		cout << *pos << endl;
		++pos;
	}
}