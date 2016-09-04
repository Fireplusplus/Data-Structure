#pragma once

#include <iostream>
#include <vector>

using namespace std;

class BitMap
{
public:
	BitMap(size_t size)
	{
		_map.resize((size >> 5) + 1);
	}

	void set(int num)
	{
		int index = num >> 5;
		int pos = num % 32;

		_map[index] |= (1 << pos);
	}

	void unset(int num)
	{
		int index = num >> 5;
		int pos = num % 32;

		_map[index] &= (~(1 << pos));
	}

	bool test(int num)
	{
		int index = num >> 5;
		int pos = num % 32;

		return (_map[index] & (1 << pos));
	}
private:
	vector<int> _map;
};

void TestBitMap()
{
	BitMap b(-1);

	b.set(1);
	b.set(10);
	b.set(1000);
	b.set(1024);
	
	cout << "1" << " " << b.test(1) << endl;
	cout << "10" << " " << b.test(10) << endl;
	cout << "1000" << " " << b.test(1000) << endl;
	cout << "1024" << " " << b.test(1024) << endl;

	cout << endl;
	b.unset(1000);

	cout << "1" << " " << b.test(1) << endl;
	cout << "10" << " " << b.test(10) << endl;
	cout << "1000" << " " << b.test(1000) << endl;
	cout << "1024" << " " << b.test(1024) << endl;
}