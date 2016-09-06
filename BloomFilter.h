#pragma once

#include "BitMap.h"
#include <iostream>
using namespace std;

struct HashFunc1
{
	size_t HashFunc(const char *str)
	{
		register size_t hash = 0;
		while (size_t ch = (size_t)*str++)
		{
			hash = hash * 131 + ch;
		}
		return hash;
	}

	size_t operator()(const char *str)
	{
		return HashFunc(str);
	}
};

struct HashFunc2
{
	size_t HashFunc(const char *str)
	{
		register size_t hash = 0;
		while (size_t ch = (size_t)*str++)
		{
			hash = 65599 * hash + ch;
			//hash = (size_t)ch + (hash << 6) + (hash << 16) - hash;  
		}
		return hash;
	}

	size_t operator()(const char *str)
	{
		return HashFunc(str);
	}
};

struct HashFunc3
{
	size_t HashFunc(const char *str)
	{
		register size_t hash = 0;
		size_t magic = 63689;
		while (size_t ch = (size_t)*str++)
		{
			hash = hash * magic + ch;
			magic *= 378551;
		}
		return hash;
	}

	size_t operator()(const char *str)
	{
		return HashFunc(str);
	}
};

struct HashFunc4
{
	size_t HashFunc(const char *str)
	{
		register size_t hash = 0;
		size_t ch;
		for (long i = 0; ch = (size_t)*str++; i++)
		{
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
			}
		}
		return hash;
	}

	size_t operator()(const char *str)
	{
		return HashFunc(str);
	}
};

struct HashFunc5
{
	size_t HashFunc(const char *str)
	{
		if (!*str)
			return 0;
		register size_t hash = 1315423911;
		while (size_t ch = (size_t)*str++)
		{
			hash ^= ((hash << 5) + ch + (hash >> 2));
		}
		return hash;
	}

	size_t operator()(const char *str)
	{
		return HashFunc(str);
	}
};


template <typename K = string, 
typename _HashFunc1 = HashFunc1,
typename _HashFunc2 = HashFunc2, 
typename _HashFunc3 = HashFunc3, 
typename _HashFunc4 = HashFunc4, 
typename _HashFunc5 = HashFunc5>
class BloomFilter
{
public:
	BloomFilter(size_t size)
		: _map(size)
		, _size(size)
	{}

	void set(const K& key)
	{
		_map.set(_HashFunc1()(key.c_str()) % _size);
		_map.set(_HashFunc2()(key.c_str()) % _size);
		_map.set(_HashFunc3()(key.c_str()) % _size);
		_map.set(_HashFunc4()(key.c_str()) % _size);
		_map.set(_HashFunc5()(key.c_str()) % _size);
	}

	bool test(const K& key)
	{
		if (_map.test(_HashFunc1()(key.c_str()) % _size) == false)
			return false;

		if (_map.test(_HashFunc2()(key.c_str()) % _size) == false)
			return false;

		if (_map.test(_HashFunc3()(key.c_str()) % _size) == false)
			return false;

		if (_map.test(_HashFunc4()(key.c_str()) % _size) == false)
			return false;

		if (_map.test(_HashFunc5()(key.c_str()) % _size) == false)
			return false;

		return true;
	}

private:
	BitMap _map;
	size_t _size;
};

void TestBloomFilter()
{
	BloomFilter<> bf(1024);
	bf.set("https://www.baidu.com/index.php?tn=monline_3_dg");
	bf.set("https://www.baidu.com/index.php?tn=monline_3_da");
	bf.set("hello world");

	cout << bf.test("https://www.baidu.com/index.php?tn=monline_3_dg") << endl;
	cout << bf.test("https://www.baidu.com/index.php?tn=monline_3_da") << endl;
	cout << bf.test("hello world") << endl;
	cout << bf.test("https://www.baidu.com/index.php?tn=monline_3_db") << endl;

}
