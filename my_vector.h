#pragma once

#include <iostream>
#include <assert.h>

using namespace std;

template <typename T>
class Vector
{
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;

	Vector()
		: _start(NULL)
		, _finish(NULL)
		, _endOfStorage(NULL)
	{}

	void PushBack(const T& val)
	{
		_CheckStorage();
		new(_finish) T(val);
		++_finish;
	}

	void PopBack()
	{
		assert(_start != _finish);
		--_finish;
	}

	void Insert(const Iterator pos, const T& val)
	{
		_CheckStorage();
		for (Iterator cur = _finish; cur != pos; --cur)
			*cur = *(cur-1);
		*pos = val;
		++_finish;
	}

	void Erase(const Iterator pos)
	{
		assert(pos >= _start && pos < _finish);
		
		--_finish;
		for (; pos != _finish; ++pos)
			*pos = *(pos+1);
	}

	size_t Size()
	{
		return _finish - _start;
	}

	T& operator[](size_t pos)
	{
		assert(pos < Size());

		return _start[pos];
	}

	Iterator Begin()
	{
		return _start;
	}

	Iterator End()
	{
		return _finish;
	}

	ConstIterator Begin() const
	{
		return _start;
	}

	ConstIterator End() const
	{
		return _finish;
	}
protected:
	void _CheckStorage()
	{
		if (_finish != _endOfStorage)
			return;
		size_t size = _finish - _start;
		size_t newStorage = size == 0 ? 1 : 2*size;
		T* tmp = new T[newStorage];
		for (size_t i = 0; i < size; i++)
			tmp[i] = _start[i];
		delete[] _start;
		_start = tmp;
		_finish = _start + size;
		_endOfStorage = _start + newStorage;
	}

protected:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
};

void TestVector()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.PushBack(5);

	Vector<int>::Iterator pos = v.Begin();
	for (; pos != v.End(); ++pos)
		cout << *pos << " ";
	cout << endl;

	for (int i = 0; i < v.Size(); i++)
		cout << v[i] << " ";
	cout << endl;
}