#include <iostream>
#include <assert.h>
#include <string.h>
#include <math.h>

void Print(int arr[], int n);

int MaxBit(int *arr, int n)
{
	int max = arr[0];
	for (int i = 1; i < n; ++i)
	{
		if (arr[i] > max)
			max = arr[i];
	}

	int bit = 0;
	while (max > 0)
	{
		++bit;
		max /= 10;
	}

	return bit;
}

void LSD(int arr[], int n)
{
	assert(NULL != arr && n > 0);
	
	int max_bit = MaxBit(arr, n);

	int *cnt = new int[10];
	int *start = new int[10];
	int *bucket = new int[n];
	
	for (int bit = 0; bit < max_bit; ++bit)
	{
		memset(cnt, 0, 10 * sizeof(int));
		memset(start, 0, 10 * sizeof(int));
		
		int adjust = (int)pow(10, (double)(bit));
		int i = 0;
		// 统计对应位出现次数 ---- // 矩阵的快速转置
		for (; i < n; ++i)
		{
			cnt[arr[i]/adjust % 10]++;
		}
	
		// 统计对应桶的起始位置
		for (i = 1; i < 10; ++i)
		{
			start[i] = start[i - 1] + cnt[i - 1]; 
		}
	
		// 元素入桶
		for (i = 0; i < n; ++i)
		{
			int bkt = arr[i]/adjust % 10;
			int addr = start[bkt];
			bucket[addr] = arr[i];
			start[bkt]++;
		}
		
		// 收集
		for (i = 0; i < n; ++i)
		{
			arr[i] = bucket[i];
		}
	}
	
	delete[] cnt;
	delete[] start;
	delete[] bucket;
}


void CountSort(int arr[], int n)
{
	assert(NULL != arr && n > 0);

	int max = arr[0];
	int min = arr[0];
	int i = 0;
	for (int i = 1; i < n; ++i)
	{
		if (arr[i] > max)
			max = arr[i];
		if (arr[i] < min)
			min = arr[min];
	}

	int size = max - min + 1;
	int *cnt = new int[size];
	memset(cnt, 0, size * sizeof(int));

	for (i = 0; i < n; ++i)
	{
		cnt[arr[i] - min]++;
	}
	
	int index = 0;
	for (i = 0; i < size; ++i)
	{
		while (cnt[i]--)
		{
			arr[index++] = i + min;
		}
	}

	delete[] cnt;
}

void MergeSortArray(int arr[], int dst[], int left, int mid, int right)
{
	int index = left;

	int l = left, r = mid;
	while (l < mid && r < right)
	{
		if (arr[l] < arr[r])
			dst[index++] = arr[l++];
		else
			dst[index++] = arr[r++];
	}
	
	while (l < mid)
		dst[index++] = arr[l++];
	while (r < right)
		dst[index++] = arr[r++];
// ---Merge1---
//	for (int index = left; index < right; index++)
//		arr[index] = dst[index];
}

void Merge(int arr[], int dst[], int left, int right)
{
	if (right - 1 > left)
	{
		int mid = ((right - left)>>1) + left;
		Merge(arr, dst, left, mid);
		Merge(arr, dst, mid, right);
		MergeSortArray(arr, dst, left, mid, right);
	}
}

void MergeSort(int arr[], int size)
{
	assert(NULL != arr && size > 0);

	int *dst = new int[size];
	Merge(arr, dst, 0, size);
	delete[] dst;
}

//------------------------------------------
void MergePass(int arr[], int dst[], int size, int len)
{
	int i, j;
	for (i = 0; i <= size - len*2; i += len*2)
	{
		MergeSortArray(arr, dst, i, i + len, i + len*2);
	}

	if (i + len < size)
		MergeSortArray(arr, dst, i, i + len, size);
	else
	{
		for (j = i; j < size; ++j)
			dst[j] = arr[j];
	}
}

void MergeSort2(int arr[], int size)
{
	assert(NULL != arr && size > 0);

	int *dst = new int[size];
	int len = 1;
	while (len < size)
	{
		MergePass(arr, dst, size, len);
		len *= 2;
		MergePass(dst, arr, size, len);
		len *= 2;	
	}

	delete[] dst;
}

void ShellSort(int arr[], int n)
{
	assert(NULL != arr && n > 0);

	for (int gap = n / 2; gap > 0; gap /= 2)
		for (int i = gap; i < n; i += gap)
		{
			int tmp = arr[i];
			int j = i - gap;
			for (; j >= 0 && tmp < arr[j]; j -= gap)
			{
				arr[j + gap] = arr[j];
			}
			arr[j + gap] = tmp;
		}
}

void InsertSort(int arr[], int n)
{
	if (NULL == arr)
		return;
	
	for (int i = 1; i < n; ++i)
	{
		int tmp = arr[i];
		int j = i - 1;
		for (; j >= 0 && arr[j] > tmp; --j)
		{
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = tmp;
	}
}


int Media(int arr[], int left, int right)
{
	int mid = ((right - left) >> 1) + left;
	if (arr[left] > arr[mid])
		std::swap(arr[left], arr[mid]);
	if (arr[left] > arr[right])
		std::swap(arr[left], arr[right]);
	
	if (arr[mid] < arr[right])
		std::swap(arr[mid], arr[right]);
	
	return right;
}

int Partition(int arr[], int left, int right)
{
	assert(NULL != arr);
	assert(left < right);

	int pivot = Media(arr, left, right);
	int small = left - 1;
	for (int i = left; i < right; ++i)
	{
		if (arr[i] < arr[pivot] && i != ++small)
		{
			std::swap(arr[small], arr[i]);
		}
	}
	++small;
	std::swap(arr[small], arr[pivot]);
	
	return small;
}

int Partition2(int arr[], int left, int right)
{
	assert(NULL != arr);
	assert(left < right);

	int pivot = Media(arr, left, right);	//pivot == right

	int i = left, j = right;		//left 已经可以确定小于right,可以跳过, 同理j不用和right比,所以后面用前置++
	while (i < j)
	{
		while (arr[++i] < arr[pivot]);	//相等也停下来交换，让i,j基本停在中间
		while (arr[--j] > arr[pivot]);

		if (i < j)
			std::swap(arr[i], arr[j]);
		else
			break;
	}
	std::swap(arr[i], arr[pivot]);
	
	return i;
}

void Qsort(int arr[], int left, int right)
{
	if (left == right)
		return;
	if (right - left < 3)
		InsertSort(arr + left, right - left + 1);
	else
	{
     	 	int mid = Partition2(arr, left, right);
     	 
     	 	if (mid > left)
     	 		Qsort(arr, left, mid - 1);
     	 	if (mid < right)
     	 		Qsort(arr, mid + 1, right);
	}
}

void QuickSort(int arr[], int size)
{
	assert(NULL != arr && size > 0);
	Qsort(arr, 0, size - 1);
}

void AdjustDown(int arr[], int size, int root)
{
	assert(NULL != arr && size > 0);

	while (root <= size/2 - 1)
	{
		int child = root*2 + 1;
		if (child + 1 < size && arr[child] < arr[child + 1])
			child++;
		
		if (arr[root] < arr[child])
		{
			std::swap(arr[root], arr[child]);
			root = child;
		}
		else 
			break;
	}
}

void HeapSort(int arr[], int n)
{
	assert(NULL != arr && n > 0);
	
	for (int i = n/2 - 1; i >= 0; --i)
	{
		AdjustDown(arr, n, i);
	}

	for (int j = n - 1; j > 0; --j)
	{
		std::swap(arr[0], arr[j]);
		AdjustDown(arr, j, 0);
	}
}


void BubbleSort(int arr[], int n)
{
	if (NULL == arr)
		return;
	
	bool flag;
	for (int i = 0; i < n - 1; ++i)
	{
		flag = false;
		for (int j = 1; j < n - i; ++j)
		{
			if (arr[j - 1] > arr[j])
			{
				std::swap(arr[j - 1], arr[j]);
				flag = true;
			}
		}
		if (!flag)
			break;
	}
}

void Binary_InsertSort(int arr[], int n)
{
	if (NULL == arr)
		return;

	for (int i = 1; i < n; ++i)
	{
		int tmp = arr[i];
		int left = 0;
		int right = n - 1;
		while (left <= right)
		{
			int mid = ((right - left) >> 2) + left;
			if (arr[mid] > tmp)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		for (int j = i - 1; j >= left; --j)
			arr[j + 1] = arr[j];
		arr[left] = tmp;
	}
}

void Print(int arr[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;;
}

void SelectSort(int arr[], int n)
{
	assert(NULL != arr && n > 0);

	int begin = 0;
	for (int i = 0; i < n; ++i)
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (arr[min] > arr[j])
				min = j;
		}
		std::swap(arr[min], arr[i]);
	}
}

int main()
{
	//int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	int arr[] = {1000, 123, 245, 125, 435, 754, 878, 745};
	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	InsertSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	InsertSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	SelectSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	BubbleSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	HeapSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	QuickSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	ShellSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	MergeSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	MergeSort2(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	QuickSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	CountSort(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

//	LSD(arr, sizeof(arr) / sizeof(arr[0]));
//	Print(arr, sizeof(arr) / sizeof(arr[0]));	

	MSD(arr, sizeof(arr) / sizeof(arr[0]));
	Print(arr, sizeof(arr) / sizeof(arr[0]));	
	return 0;
}
