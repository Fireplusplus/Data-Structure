#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename K, typename V>
struct HashNode
{
	K _key;
	V _val;

	HashNode* _next;

	HashNode(const K &key, const V &val)
		: _key(key)
		, _val(val)
	{}
};

template <typename K>
struct _KeyToInt
{
	K operator()(const K& key) const
	{
		return key;
	}
};

template <>
struct _KeyToInt<string>
{
	int operator()(const string& str) const
	{
		return BKDRHash(str.c_str());
	}

private: 
	static size_t BKDRHash(const char * str)
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313     
		unsigned int hash = 0;
		while (*str)
		{
			hash = hash * seed + (*str++);
		}
		return (hash & 0x7FFFFFFF);
	}
};



template <typename K, typename V, typename ToInt = _KeyToInt<K> >
class HashTableBucket
{
	typedef HashNode<K, V> Node;
public:
	HashTableBucket(size_t size = 0)
		: _size(0)
	{
		_tables.resize(GetNextPrime(size));
	}

	Node * Find(const K& key)
	{
		int index = _HashFunc(key, _tables.size());
		Node *cur = _tables[index];
		
		while (NULL != cur)
		{
			if (cur->_key == key)
				return cur;
			cur = cur->_next;
		}
		return NULL;
	}

	bool Insert(const K& key, const V& val)
	{
		CheckCapacity();
		if (Find(key) != NULL)
			return false;

		Node *tmp = new Node(key, val);
		int index = _HashFunc(key, _tables.size());
		tmp->_next = _tables[index];
		_tables[index] = tmp;

		_size++;
		return true;
	}

	bool Remove(const K& key)
	{
		int index = _HashFunc(key, _tables.size());
		Node *cur = _tables[index];
		Node *prev = NULL;

		while (NULL != cur)
		{
			if (cur->_key == key)
				break;
			else
			{
				prev = cur;
				cur = cur->_next;
			}
		}
		if (NULL == cur)	// 没找到
			return false;

		if (prev == NULL)	// 删除第一个结点
		{
			_tables[index] = cur->_next;
			delete cur;
		}
		else
		{
			prev->_next = cur->_next;
			delete cur;
		}
		return true;
	}

	void Print()
	{
		for (int i = 0; i < _tables.size(); i++)
		{
			Node *cur = _tables[i];
			printf("_tables[%d]", i);
			while (NULL != cur)
			{	
				cout << cur->_key << "-";
				cout << cur->_val << " ";
				cur = cur->_next;
			}
			cout << endl;
		}
	}

private:
	unsigned long GetNextPrime(int size)
	{
		const int _PrimeSize = 28;     
		static const unsigned long _PrimeList[_PrimeSize] = { 
			53ul, 97ul, 193ul, 389ul, 769ul, 1543ul, 3079ul, 6151ul, 
			12289ul, 24593ul, 49157ul, 98317ul, 196613ul, 393241ul, 
			786433ul, 1572869ul, 3145739ul, 6291469ul, 12582917ul, 
			25165843ul, 50331653ul, 100663319ul, 201326611ul,
			402653189ul, 805306457ul, 1610612741ul, 
			3221225473ul, 4294967291ul 
		};

		for (int i = 0; i < _PrimeSize; i++)
		{
			if (size < _PrimeList[i])
				return _PrimeList[i];
		}
		return _PrimeList[_PrimeSize-1];
	}

	void CheckCapacity()
	{
		// 载荷因子为1
		if (_size == _tables.size())
		{
			int size = GetNextPrime(_tables.size());
			vector<Node *> new_tables(size);
			
			for (int i = 0; i < _tables.size(); i++)
			{
				Node *cur = _tables[i];
				
				while (NULL != cur)
				{
					Node *next = cur->_next;

					int index = _HashFunc(cur->_key, size);
					cur->_next = new_tables[index];
					new_tables[index] = cur;

					cur = next;
				}

				_tables[i] = NULL;
			}
			_tables.swap(new_tables);
		}
	}

	int _HashFunc(const K& key, int size)
	{
		return ToInt()(key) % size;
	}

private:
	vector<Node *> _tables;
	size_t _size;
};

void TestHashTableBucket()
{
	HashTableBucket<int, int> ht;

	int arr[] = {51, 105, 52, 3, 55, 2, 106, 53, 0};
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		ht.Insert(arr[i], i);
	}
	ht.Print();
}

void TestHashTableBucket2()
{
	HashTableBucket<string, string> ht;

	ht.Insert("sort", "排序");
	ht.Insert("insert", "插入");
	ht.Insert("search", "查找");
	ht.Insert("remove", "删除");
	ht.Insert("erase", "删除");

	ht.Print();
}