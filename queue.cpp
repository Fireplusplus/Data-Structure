#include <iostream>

using namespace std;

////顺序队列:1.pop元素时后边元素前移；2.front后移(产生问题伪溢出--->
//					解决方法：循环队列)
//				环形队列判空判满方法:1.少用一个空间；2.计数器；3.标记，标记上次操作为push或pop，配合头尾指针判断
//template <typename T>		//front rear(const, 非const)
//class Queue
//{
//public:
//	Queue(size_t capacity = 10)
//		: _front(NULL)
//		, _rear(NULL)
//		, _capacity(10)
//		, _size(0)
//	{
//		_capacity = capacity > _capacity ? capacity : _capacity;
//		_arr = new T[_capacity];
//	}
//
//	void Push(const T &data)
//	{
//		//check capacity
//		if (_rear == _capacity)
//			return;
//		_arr[_rear++] = data;
//	}
//
//	void Pop()
//	{
//		//check empty
//		if (!Empty())
//		{
//			for (int i = 1; i < _rear; ++i)
//			{
//				_arr[i - 1] = _arr[i];
//			}
//			--_rear;
//		}
//	}
//
//	bool Empty()
//	{
//		return 0 == _rear;
//	}
//	
//	~Queue()
//	{
//		if (NULL != _arr)
//		{
//			delete[] _arr;
//		}
//	}
//private:
//	T *_arr;
//	int _front;
//	int _rear;
//	size_t _capacity;
//};


int main()
{

	return 0;
}
