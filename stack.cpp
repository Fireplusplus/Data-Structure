#include <iostream>
#include <assert.h>
#include <stack>

using namespace std;

template <typename T>
class Stack
{
private:
	void check_capacity()
	{
		if (_size == _capacity)
		{
			_capacity *= 2;
			T *tmp = new T[_capacity];
			
			//memcopy-->浅拷贝
			//类型萃取
			for (int i = 0; i < _size; ++i)
			{
				tmp[i] = _pData[i];
			}
			delete[] _pData;
			_pData = tmp;
		}
	}

public:
	Stack()
		: _size(0)
		, _capacity(3)
		, _pData(new T[_capacity])
	{}

	Stack(const Stack &stack)
	{
		_capacity = stack._capacity;
		_pData = new T[_capacity];
		_size = stack._size;

		for (int i = 0; i < _size; ++i)
		{
			_pData[i] = stack._pData[i];
		}
	}

//	Stack & operator=(const Stack &stack)
//	{
//		if (this != &stack)
//		{
//			if (0 != _capacity)
//			{
//				delete[] _pData;
//			}
//			_capacity = stack._capacity;
//			_pData = new T[_capacity];
//			_size = stack._size;
//
//			for (int i = 0; i < _size; ++i)
//			{
//				_pData[i] = stack._pData[i];
//			}
//
//		return *this;
//	}

	Stack & operator=(const Stack stack)
	{
		std::swap(_pData, stack._pData);
		std::swap(_size, stack._size);
		std::swap(_capacity, stack._capacity);
	}


	void Push(const T &data)
	{
		check_capacity();
		_pData[_size++] = data;	
	}

	void Pop()
	{
		assert(0 != _size);
		_size--;
	}

	bool Empty() const
	{
		return 0 == _size;
	}

	T & Top()
	{
		if (0 == size)
			assert(0);
		return _pData[_size - 1];
	}

	size_t Size() const
	{
		return _size;
	}

	~Stack()
	{
		if (NULL != _pData)
		{
			delete[] _pData;
			_size = 0;
			_capacity = 0;
		}
	}

private:
	int _size;
	int _capacity;
	T *_pData;
};

//顺序栈和链式栈的优缺点：程序的局部性原理, 内存碎片

//括号匹配问题

int MatchBrackets(char *str)
{
	if (NULL == str)
		return -1;
	stack<char> stack;

	char *pos = str;

	while (*pos)
	{
		if ('(' == *pos || '[' == *pos || '{' == *pos)
			stack.push(*str);
		//else
	}


}

int main()
{
	stack<int> s;
//	cout << s.top() << endl;	//error:段错误


	return 0;
}
