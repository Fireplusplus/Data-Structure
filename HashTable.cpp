#include <iostream>
#include <string>
#include <string.h>

enum Status
{
	EMPTY, 
	EXISTS,
	DELETE	
};

template <typename K>
struct __HashFunc
{
	size_t operator()(const K &key)
	{
		return key;
	}
};

template <>
struct __HashFunc<std::string>
{
	static size_t BKDRHash(const char *str)
	{
		unsigned int seed = 131;
		unsigned int hash = 1;
		while (*str)
		{
			hash = hash * seed + (*str++);
		}

		return (hash & 0x7fffffff);
	}

	size_t operator()(const std::string &str)
	{
		return BKDRHash(str.c_str());
	}
};

template <typename K, typename V, typename HashFunc = __HashFunc<K> >
class HashTable
{
public:
	HashTable(size_t capacity)
		: _tables(new std::pair<K, V>[capacity])
		, _status(new Status[capacity])
		, _size(0)
		, _capacity(capacity)
	{
		memset(_tables, '\0', sizeof(std::pair<K, V>) * capacity);
		for (size_t i = 0; i < capacity; i++)
		{
			_status[i] = EMPTY;
		}
	}

	std::pair<K, V> * Find(const K &key)
	{
		int i = 0;
		int index = HashFunci(key, i);

		while (_status[index] != EMPTY)
		{
			if (_tables[index].first == key && _status[index] != DELETE)
				return _tables + index;
			else
				index = HashFunci(key, ++i);
		}

		return NULL;
	}

	bool Insert(const K &key, const V &value)
	{
		if (NULL != Find(key))	
			return false;
		CheckCapacity();
		
		size_t i = 0;
		size_t index = HashFunci(key, i);
		while (_status[index] == EXISTS)
		{
			index = HashFunci(key, ++i);
		}

		_tables[index].first = key;
		_tables[index].second = value;
		++_size;
		_status[index] = EXISTS;
		
		return true;
	}

	~HashTable()
	{
		if (_tables)
		{
			delete[] _tables;
			delete[] _status;
		}
	}
	
	void Erase(const K &key)
	{
		std::pair<K, V> *ret = Find(key);
		if (NULL == ret)
			return;
		size_t index = ret - &tables;
		_status[index] = DELETE;	
	}
		
	void Print()
	{
		for (size_t i = 0; i < _capacity; i++)
		{
			std::cout << _tables[i].first << " ";
		}
		std::cout << std::endl;
	}

private:
	// 闭散列/开放定址法--: 二次探测解决冲突
	size_t HashFunci(const K &key, size_t i)
	{
		HashFunc func;
		return (func(key) + i*i) % _capacity;
	}
	

	void CheckCapacity()
	{
		// 当负载因子(size/capacity)到0.7，则进行增容	
		if ((_capacity == 0) || (_size*10 / _capacity == 7))
		{
			size_t newCapacity = 2 * _capacity;
			HashTable ht(newCapacity);
			if (NULL != _tables)
			{
				size_t i = 0;
				while (i < _capacity)
				{
					if (_status[i] == EXISTS)
						ht.Insert(_tables[i].first, _tables[i].second);
					++i;
				}

			}
		}
	}

private:
	std::pair<K, V> *_tables;
	Status *_status;
	size_t _size;
	size_t _capacity;
};

void test()
{
	HashTable<int, int> ht(10);
	int a[] = {89, 18, 49, 58, 9};
	for (size_t i = 0; i < sizeof(a)/sizeof(a[0]); i++)
	{
		ht.Insert(a[i], i);
	}
	ht.Print();

	HashTable<std::string, std::string> dict(10);
	dict.Insert("sort", "排序");
	std::pair<std::string, std::string> *ret = dict.Find("sort");
	if (NULL != ret)
		std::cout << ret->second << std::endl;

}


int main()
{
	test();

	return 0;
}






