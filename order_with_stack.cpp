#include <iostream>
#include <stack>
#include <string.h>

//出入栈顺序匹配问题
//12345---43521

bool IsMatch(char *str_in, char *str_out)
{
	if (NULL == str_in || NULL == str_out)
		return false;
	
	if (strlen(str_in) != strlen(str_out))
		return false;

	std::stack<char> s;
	
	while (*str_in)
	{
		while ('\0' != *str_in && *str_in != *str_out)
		{
			s.push(*str_in);
			++str_in;
		}
		if ('\0' == *str_in)
			break;
		++str_in;
		++str_out;
	
		while (!s.empty() && s.top() == *str_out)
		{
			s.pop();
			++str_out;
		}
	}
	if ('\0' == *str_out)
		return true;
	return false;
}


int main()
{
	char *s1 = (char *)"12345";
	char *s2 = (char *)"42321";
	
	if (IsMatch(s1, s2))
		std::cout << "match!" << std::endl;
	else
		std::cout << "not match!" << std::endl;
	return 0;
}
