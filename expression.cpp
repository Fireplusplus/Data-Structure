#include <iostream>
#include <stack>
#include <assert.h>
#include <string>

using namespace std;

int get_prm_out(char c)
{
	switch (c)
	{
	case '+':
	case '-':
		return 2;
	case '*':
	case '/':
		return 4;
	case '(':
		return 6;
	case ')':
		return 1;
	case '#':
		return 0;
	default:
		break;
	}

	return 0;
}

int get_prm_in(char c)
{
	switch (c)
	{
	case '+':
	case '-':
		return 3;
	case '*':
	case '/':
		return 5;
	case '(':
		return 1;
	case ')':
		return 6;
	case '#':
		return 0;
	default:
		break;
	}

	return 0;
}

string change_expression(const char *exp)
{
	if (NULL == exp)
		return string();
	
	stack<char> s;
	s.push('#');
	string rs;
	while ('\0' != *exp)
	{
		bool flag = false;
		while (*exp >= '0' && *exp <= '9')
		{
			rs += *exp;
			exp++;
			flag = true;
		}
		if (flag)
		{
			rs += " ";
		}

		if (get_prm_out(*exp) > get_prm_in(s.top()))
		{
			s.push(*exp);
			++exp;
		}
		else
		{
			if ('(' != s.top())
				rs += s.top();
			else
				++exp;
			s.pop();
		}
	}
	while (!s.empty() && s.top() != '#')
	{
		rs += s.top();
		s.pop();
	}

	return rs;
}

int comput(const char *exp)
{	
	if (NULL == exp)
	{
		assert(false);
		return 0;
	}

	stack<int> s;
	while ('\0' != *exp)
	{
		int flag = false;
		int num = 0;
		while (*exp >= '0' && *exp <= '9')
		{
			num = num * 10 + *exp - '0'; 	
			++exp;
			flag = true;
		}
		if (flag)
		{
			++exp;
			s.push(num);
			continue;
		}
		
		if (*exp != '\0')
		{
			int right = s.top();
			s.pop();
			int left = s.top();
			s.pop();
			if ('-' == *exp)
			{
				s.push(left - right); 
			}
			else if ('+' == *exp)
			{
				s.push(left + right); 
			}
			else if ('*' == *exp)
			{
				s.push(left * right); 
			}
			else if ('/' == *exp)
			{
				s.push(left / right); 
			}
			++exp;
		}
	}
	
	return s.top();
}

int main()
{
	const char *str = "12+4*(2-1)+6/3";
	cout << str << endl;
	
	string exp = change_expression(str);
	cout << exp << endl;
	
	int ret = comput(exp.c_str());
	cout << ret << endl;

	return 0;
}
