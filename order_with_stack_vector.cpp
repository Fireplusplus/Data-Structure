#include <iostream>
#include <stack>
#include <vector>

//出入栈顺序匹配问题
//12345---43521
bool IsMatch(std::vector<int> &v_in, std::vector<int> &v_out)
{
	if (v_in.empty() || v_out.empty() || v_in.size() != v_out.size())
		return false;
		
	std::stack<int> s;
	
	int index_in = 0, index_out = 0;
	while (index_in < v_in.size())
	{
		while (index_in < v_in.size() && v_in[index_in] != v_out[index_out])
		{
			s.push(v_in[index_in++]);
		}

		++index_in;
		++index_out;

		while (!s.empty() && s.top() == v_out[index_out])
		{
			s.pop();
			++index_out;
		}
		if (s.empty() && index_out == v_out.size())
			return true;
	}
	
	return false;
}


int main()
{
	std::vector<int> v1, v2;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	v1.push_back(5);

	v2.push_back(4);
	v2.push_back(3);
	v2.push_back(5);
	v2.push_back(1);
	v2.push_back(2);

	if (IsMatch(v1, v2))
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
		
	return 0;
}
