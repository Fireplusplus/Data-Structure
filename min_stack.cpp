#include <iostream>
#include <stack>
#include <assert.h>

template <typename T>
class min_stack
{
public:
	void Push(const T &data)
	{
		s.push(data);
		if (min.empty() || min.top() >= data)
			min.push(data);
	}

	void Pop()
	{
		assert(!s.empty());
		
		if (s.top() == min.top())
		{
			s.pop();
			min.pop();
		}
		else
			s.pop();
	}

	T & Top()
	{
		return s.top();
	}

	T Min()
	{
		return min.top();
	}

private:
	std::stack<T> s;	
	std::stack<T> min;	
};

int main()
{
	min_stack<int> s;
	s.Push(2);
	std::cout << s.Top() << " " << s.Min() << std::endl;
	s.Push(3);
	std::cout << s.Top() << " " << s.Min() << std::endl;
	s.Push(1);
	std::cout << s.Top() << " " << s.Min() << std::endl;
	s.Push(1);
	std::cout << s.Top() << " " << s.Min() << std::endl;

	s.Pop();
	std::cout << s.Top() << " " << s.Min() << std::endl;
	s.Pop();
	std::cout << s.Top() << " " << s.Min() << std::endl;
	s.Pop();
	std::cout << s.Top() << " " << s.Min() << std::endl;
	
	return 0;
}
