#include <iostream>
#include <assert.h>
#include <vector>

//对称矩阵压缩存储
template <typename T>
class symmetric_matrix
{
public:
	symmetric_matrix(T *arr = NULL, size_t line = 0)
		: _line(line)
		, _size(_line * (_line + 1) / 2) 
	{
		if (NULL == arr)
		{
			assert(NULL != arr);
			return;
		}
		for (int i = 0; i < _line; ++i)
		{
			for (int j = 0; j < _line; ++j)
			{
				if (i >= j)
				{
					_matrix.push_back(arr[i * _line + j]);
				}
			}
		}
	}

	T access(size_t row, size_t col)
	{
		assert(row >= 0 && row < _line);
		assert(col >= 0 && col < _line);

		if (row < col)
		{
			std::swap(row, col);
		}
	
		return _matrix[row * (row + 1) / 2 + col];
	}

	friend std::ostream & operator<<(std::ostream &os, symmetric_matrix &m)
	{
		for (int i = 0; i < m._line; ++i)
		{
			for (int j = 0; j < m._line; ++j)
			{
				os << m.access(i, j) << " ";
			}
			os << std::endl;
		}
		return os;
	}

private:
	size_t _line;
	size_t _size;
	std::vector<T> _matrix;
};

int main()
{
	int arr[5][5] = {
		{0, 1, 2, 3, 4}, 
		{1, 0, 1, 2, 3},
		{2, 1, 0, 1, 2},
		{3, 2, 1, 0, 1},
		{4, 3, 2, 1, 0}
	};
	
	symmetric_matrix<int> m((int *)arr, 5);

	std::cout << m << std::endl;

	return 0;
}
