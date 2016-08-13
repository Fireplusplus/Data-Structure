#include <iostream>

template <typename T>
class circle_queue
{
private:
	bool Fall()
	{
		if ((_rear + 1) % _capacity == _front)
			return true;
		return false;
	}

public:
	circle_queue(int capacity = 4)
		: _arr(new T[capacity])
		, _front(0)
		, _rear(0)
		, _capacity(capacity)
	{}

	void Push(const T &data)
	{
		if (!Fall())
		{
			_arr[_rear] = data;
			_rear = (_rear + 1) % _capacity;
		}
	}

	bool Empty()
	{
		return _rear == _front;
	}

	void Pop()
	{
		if (!Empty())
		{
			_front = (_front + 1) % _capacity;
		}
	}

	T &Front()
	{
		if (!Empty())
			return _arr[_front];
		else
			throw -1;
	}

	T& Rear()
	{
		if (!Empty())
			return _arr[(_rear + _capacity - 1) % _capacity];
		else
			throw -1;
	}
	
	size_t Size()
	{
		return (_rear + _capacity - _front) % _capacity;
	}

private:
	T *_arr;
	int _front;
	int _rear;
	int _capacity;
};

int main()
{
	circle_queue<int> q;

	q.Push(1);
	//std::cout << q.Rear() << std::endl;
	q.Push(2);
	//std::cout << q.Rear() << std::endl;
	q.Push(3);
	//std::cout << q.Rear() << std::endl;
	//q.Push(4);
	//std::cout << q.Rear() << std::endl;
	
	q.Pop();
	//std::cout << q.Rear() << std::endl;
	std::cout << q.Front() << std::endl;
	
	q.Pop();
	std::cout << q.Front() << std::endl;
	
	q.Pop();
	std::cout << q.Front() << std::endl;
	

	return 0;
}
