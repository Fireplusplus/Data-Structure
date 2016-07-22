#include <iostream>
#include <vector>
#include <assert.h>

//稀疏矩阵压缩存储
template <typename T>
struct Data
{
	Data(int row = 0, int col = 0, const T &val = 0)
		: _r(row)
		, _c(col)
		, _val(val)
	{}

	int _r;
	int _c;
	T _val;
};

template <typename T>
class sparse_matrix
{	
	template <typename T1>
	friend sparse_matrix<T1> operator+(const sparse_matrix<T1> &left, const sparse_matrix<T1> &right);

public:
	sparse_matrix()
	{}

	sparse_matrix(T *arr, size_t row, size_t col, const T &invalid)
		: _row(row)
		, _col(col)
		, _invalid(invalid)
	{
		if (NULL == arr)
			return;
		for (int i = 0; i < _row; ++i)
		{
			for (int j = 0; j < _col; j++)
			{
				if (arr[i * _col + j] != _invalid)
				{
					_matrix.push_back(Data<T>(i, j, arr[i * _col + j]));
				}
			}
		}
	}

	T access(size_t row, size_t col)
	{
		assert(row >= 0 && row < _row);		
		assert(col >= 0 && col < _col);		
		
		int index = 0;
		while (index < _matrix.size())
		{
			if (_matrix[index]._r == row && _matrix[index]._c == col)
			{
				return _matrix[index]._val;
			}
			++index;
		}
		return _invalid;
	}

	Data<T> accessData(size_t row, size_t col)
	{
		assert(row >= 0 && row < _row);		
		assert(col >= 0 && col < _col);		
		
		int index = 0;
		while (index < _matrix.size())
		{
			if (_matrix[index]._r == row && _matrix[index]._c == col)
			{
				return _matrix[index];
			}
			++index;
		}
		return Data<T>(row, col, _invalid);
	}

	friend std::ostream & operator<<(std::ostream &os, sparse_matrix &m)
	{
		for (int i = 0; i < m._row; ++i)
		{
			for (int j = 0; j < m._col; ++j)
			{
				os << m.access(i, j) << " ";
			}
			os << std::endl;
		}
		return os;
	}

	sparse_matrix<T> transposition()
	{
		sparse_matrix<T> ret(NULL, _row, _col, _invalid);
		
		int index = 0;
		for (int i = 0; i < _col; ++i)
		{
			for (int j = 0; j < _row; ++j)
			{
				if (index < _matrix.size())
				{
					Data<T> tmp = accessData(j, i);
					if (tmp._c == i && tmp._r == j && tmp._val != _invalid)
					{
						std::swap(tmp._c, tmp._r);
						ret._matrix.push_back(tmp);
						++index;
					}
				}
			}
		}
		return ret;
	}
	
	sparse_matrix<T> quick_transposition()
	{
		std::vector<int> num(_col, 0);
		std::vector<int> addr(_col, 0);

		//统计转置后每行数据元素的个数
		for (int i = 0; i < _matrix.size(); ++i)
		{
			num[_matrix[i]._c]++;
		}
		
	//	for (int i = 0; i < _col; ++i)
	//		std::cout << num[i] << " ";
	//	std::cout << std::endl;

		//统计转置后矩阵每行在压缩矩阵中存储的位置
		for (int i = 1; i < _col; ++i)
		{
			addr[i] = addr[i - 1] + num[i - 1];
		}
	//	for (int i = 0; i < _col; ++i)
	//		std::cout << addr[i] << " ";
	//	std::cout << std::endl;
		
		//将对应的元素放到对应的位置
		sparse_matrix<T> ret(NULL, _col, _row, _invalid);
		ret._matrix.resize(_matrix.size());
		for (int i = 0; i < _matrix.size(); ++i)
		{
			int index = addr[_matrix[i]._c]++;
			Data<T> tmp = _matrix[i];
			std::swap(tmp._r, tmp._c);
			ret._matrix[index] = tmp; 	
		}	
		return ret;
	}

private:

	std::vector<Data<T> > _matrix;
	size_t _row;
	size_t _col;
	T _invalid;
};

template <typename T1>
sparse_matrix<T1> operator+(const sparse_matrix<T1> &left, const sparse_matrix<T1> &right)
{
	if (left._invalid != right._invalid)
		return sparse_matrix<T1>(); 

	sparse_matrix<T1> sum;
	sum._row = left._row;
	sum._col = left._col;
	sum._invalid = left._invalid;

	int i_l = 0, i_r = 0;
	while (i_l < left._matrix.size() && i_r < right._matrix.size())
	{
		int l_r = left._matrix[i_l]._r;
		int r_r = right._matrix[i_r]._r;
		int l_c = left._matrix[i_l]._c;
		int r_c = right._matrix[i_r]._c;   
	     	int l_v = left._matrix[i_l]._val;
		int r_v = right._matrix[i_r]._val;

		if (l_r == r_r && l_c == r_c)
		{
			int s = l_v + r_v;
			if (s != left._invalid)
			{
				Data<T1> d(l_r, l_c, s);
				sum._matrix.push_back(d);
			}
			i_l++;
			i_r++;
		}
		else if ((l_r == r_r && l_c < r_c) || l_r < r_r)
		{
			Data<T1> d(l_r, l_c, l_v);
			sum._matrix.push_back(d);
			i_l++;
		}
		else //if ((l_r == r_r && l_c > r_c) || l_r > r_r)
		{
			Data<T1> d(r_r, r_c, r_v);
			sum._matrix.push_back(d);
			i_r++;
		}
	}

	while (i_l < left._matrix.size())
	{
		int l_r = left._matrix[i_l]._r;
		int l_c = left._matrix[i_l]._c;
		Data<T1> d(l_r, l_c, left._matrix[i_l++]._val);
		sum._matrix.push_back(d);
	}

	while (i_r < right._matrix.size())
	{
		int r_r = right._matrix[i_r]._r;
		int r_c = right._matrix[i_r]._c;   
		Data<T1> d(r_r, r_c, right._matrix[i_r++]._val);
		sum._matrix.push_back(d);
	}
	
	return sum;
}

int main()
{
	int arr[5][5] = {
		{1, 0, 3, 0, 4}, 
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{1, 0, 3, 0, 4},
		{0, 2, 0, 0, 0}
	};
	int arr2[5][5] = {
		{1, 0, 3, 0, 4}, 
		{0, 0, 0, 0, 0},
		{2, 3, 4, 0, 0},
		{1, 0, 3, 0, 4},
		{0, 2, 0, 0, 9}
	};
	
	sparse_matrix<int> m((int *)arr, 5, 5, 0);
	sparse_matrix<int> m2((int *)arr2, 5, 5, 0);
	
	std::cout << m << std::endl;
	std::cout << m2 << std::endl;
	
	sparse_matrix<int> sum = m + m2;
	std::cout << sum << std::endl;


//	sparse_matrix<int> ret = m.transposition();
//	std::cout << ret << std::endl;

//	sparse_matrix<int> ret2 = m.quick_transposition();
//	std::cout << ret2 << std::endl;

	return 0;
}
