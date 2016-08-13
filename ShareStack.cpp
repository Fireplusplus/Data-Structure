#include <iostream>
#include <assert.h>

//共享栈
//s1: 1, 2, 3    3, 2, 1 :s2

template <typename T>
class ShareStack
{
private:
	void check_capacity()
	{
		if (_size_l + _size_r == _capacity)
		{
			_capacity *= 2;
			T *tmp = new T[_capacity];
			
			int i = 0;
			for (; i < _size_l; ++i)
			{
				tmp[i] = _pData[i];
			}
			for (i = 0; i < _size_r; ++i)
			{
				tmp[_capacity - 1 - i] = _pData[_capacity / 2 - 1 - i];
			}
			delete[] _pData;
			_pData = tmp;
		}
	}

public:
	ShareStack()
		: _size_l(0)
		, _size_r(0)
		, _capacity(5)
		, _pData(new T[_capacity])
	{}

	void Push(const T &data, bool flag)	//flag 作为标记 true左false右
	{
		check_capacity();
		if (true == flag)
		{
			if (_size_l != _capacity - _size_l)
				_pData[_size_l++] = data;
		}
		else
		{
			if (_size_r != _capacity - _size_l)
				_pData[_capacity - 1 - _size_r++] = data;
		}
	}

	void Pop(bool flag)
	{
		if (true == flag)
		{
			if (0 == _size_l)
				assert(0);
			_size_l--;
		}
		else
		{
			if (0 == _size_r)
				assert(0);
			_size_r--;
		}
	}

	int Size(bool flag)
	{
		if (true == flag)
			return _size_l;
		else
			return _size_r;
	}

	bool Empty(bool flag)
	{
		if (true == flag && 0 == _size_l)
			return true;
		if (false == flag && 0 == _size_r)
			return true;
		
		return false;
	}

	T & top(bool flag)
	{
		if (true == flag)
		{
			assert(0 != _size_l);
			return _pData[_size_l];
		}
		else
		{
			assert(0 != _size_r);
			return _pData[_capacity - _size_r];
		}
		
	}

	~ShareStack()
	{
		if (NULL != _pData)
		{
			delete[] _pData;
			_size_l = 0;
			_size_r = 0;
			_capacity = 0;
		}
	}

private:
	int _size_l;
	int _size_r;
	int _capacity;
	T *_pData;
};

int main()
{
	ShareStack<int> s;
	s.Push(1, true);
	s.Push(2, true);
	s.Push(3, true);
	
	s.Push(1, false);
	s.Push(2, false);
	s.Push(3, false);

	s.Pop(true);
	s.Pop(true);
	s.Pop(true);
	s.Pop(false);
	s.Pop(false);
	s.Pop(false);

	return 0;
}
