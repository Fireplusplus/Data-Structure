#pragma once

#include <stdarg.h>
#include <string>

#define __DEBUG__

static string GetFileName(const string& path)
{
	char ch = '/';

#ifdef _WIN32
	ch = '\\';
#endif

	size_t pos = path.rfind(ch);
	if (pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}
//���ڵ���׷�ݵ�trace log
inline static void __trace_debug(const char* function,
	const char* filename, int line, char* format, ...)
{
#ifdef __DEBUG__
	//������ú�������Ϣ
	fprintf(stdout, "��%s:%d��%s", GetFileName(filename).c_str(), line, function);

	//����û����trace��Ϣ
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
#endif
}

#define __TRACE_DEBUG(...)  \
__trace_debug(__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__);

typedef void(*HandlerFunc)();

// һ���ռ�������
template <int inst>
class __MallocAllocTemplate
{
private:
	static void* oomMalloc(size_t size)
	{		
		while (1)
		{
			if (__malloc_alloc_oom_handler == NULL)
			{
				throw bad_alloc();
			}
			else
			{
				(*__malloc_alloc_oom_handler)();
				void* ret = malloc(size);
				if (ret != NULL)
					return ret;
			}
		}
	}

private:
	static HandlerFunc __malloc_alloc_oom_handler;

public:
	static void* Allocate(size_t n)
	{
		__TRACE_DEBUG("һ���ռ������������ڴ�:%d\n", n);

		void* result = malloc(n);
		if (NULL == result)
			result = oomMalloc(n);

		return result;
	}

	static void Deallocate(void* p, size_t /*n*/)
	{
		free(p);
	}

	//static void (* set_malloc_handler(void (*f)()))()
	static HandlerFunc SetMallocHandler(HandlerFunc f)
	{
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		
		return old;
	}
};

template <int inst>
void(*__MallocAllocTemplate<inst>::__malloc_alloc_oom_handler)() = 0;

// �����ռ�������
template <bool threads, int inst>
class __DefaultAllocTemplate
{
private:
	enum {__ALIGN = 8};
	enum {__MAX_BYTES = 128};
	enum {__NFREELISTS = __MAX_BYTES / __ALIGN };

	union Obj
	{
		union Obj* _freeListLink;
		char client_data[1];	 /* The client sees this.        */
	};

	static Obj* _freeList[__NFREELISTS];

	// Chunk allocation state
	static char* _startFree;	// �ڴ�صĿ�ʼ
	static char* _endFree;		// �ڴ�صĽ���
	static size_t _heapSize;	// �ѷ����ڴ�Ĵ�С

	static size_t FREELIST_INDEX(size_t bytes)
	{	
		return ((bytes + __ALIGN - 1) / __ALIGN - 1);
	}

	static size_t ROUND_UP(size_t bytes)
	{
		return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
	}

public:
	static char* ChunkAlloc(size_t n, size_t& nobjs)
	{
		char* ret = NULL;
		size_t totalBytes = n * nobjs;
		size_t bytesLeft = _endFree - _startFree;

		// �ڴ�����㹻nobjs��������ڴ�
		if (bytesLeft >= totalBytes)
		{
			__TRACE_DEBUG("�ڴ�����㹻nobjs:%d��������ڴ�\n", nobjs);

			ret = _startFree;
			_startFree += totalBytes;

			return ret;
		}
		// �ڴ��ֻ������n��������ڴ棬����nobjs
		else if (bytesLeft >= n)
		{
			nobjs = bytesLeft / n;

			__TRACE_DEBUG("ֻ��nobjs:%d��������ڴ�\n", nobjs);

			totalBytes = nobjs * n;
			ret = _startFree;
			_startFree += totalBytes;

			return ret;
		}
		// �ڴ����һ��������ڴ涼���䲻����
		else
		{
			// ������ϵͳ������ֽ���
			size_t bytesToGet = totalBytes * 2 + ROUND_UP(_heapSize >> 4);

			__TRACE_DEBUG("��ϵͳ����%d�ֽڵ��ڴ�\n", bytesToGet);

			// �����ڴ��ʣ����ֽ���
			if (bytesLeft > 0)
			{
				size_t index = FREELIST_INDEX(bytesLeft);
				((Obj*)_startFree)->_freeListLink = _freeList[index];
				_freeList[index] = (Obj*)_startFree;
			}

			_startFree = (char*)malloc(bytesToGet);
			// ����ʧ��
			if (_startFree == NULL)
			{
				// �ڸ��������������ȥȡ
				for (size_t i = n; i < __MAX_BYTES; i += __ALIGN)
				{
					size_t index = FREELIST_INDEX(i);
					if (_freeList[index] != NULL)
					{
						_startFree = (char*)_freeList[index];
						_freeList[index] = ((Obj*)_startFree)->_freeListLink;
						_endFree = _startFree + i;
						return ChunkAlloc(n, nobjs);
					}
				}

				// ɽ��ˮ��-->����һ���ռ�������
				_endFree = NULL;	// �˴��п����׳��쳣�������ܲ����쳣����֤_startFree��_endFree���
				_startFree = (char*)__MallocAllocTemplate<0>::Allocate(bytesToGet);
			}

			// malloc��һ���ռ�����������ɹ�
			_endFree = _startFree + bytesToGet;
			_heapSize = bytesToGet;
			return ChunkAlloc(n, nobjs);
		}
	}

	static void* Refill(size_t n)
	{
		// ����nobjs������Ĵ�С
		size_t nobjs = 20;
		char* chunk = ChunkAlloc(n, nobjs);
		if (n == 1)
			return chunk;

		__TRACE_DEBUG("���ڴ�鵽��������nobjs:%d\n", nobjs);
		// ���ڴ��
		Obj* cur, *next;
		size_t index = FREELIST_INDEX(n);
		cur = (Obj*)(chunk + n);
		_freeList[index] = cur;
		for (size_t i = 1; i < nobjs - 1; i++)
		{
			next = (Obj*)((char*)cur + n);
			cur->_freeListLink = next;
			cur = next;
		}
		cur->_freeListLink = NULL;
		
		return chunk;
	}

	static void* Allocate(size_t n)
	{
		__TRACE_DEBUG("�����ռ������������ڴ�:%d\n", n);

		// ����128����һ���ռ�����������
		if (n > __MAX_BYTES)
		{
			return __MallocAllocTemplate<inst>::Allocate(n);
		}

		size_t index = FREELIST_INDEX(n);
		// ���������Ӧλ��û��-->���
		if (_freeList[index] == NULL)
		{
			return Refill(ROUND_UP(n));
		}
		// ��-->ͷɾ
		else
		{
			__TRACE_DEBUG("������������ȥȡ�ڴ��:%d\n", index);

			Obj* first = _freeList[index];
			_freeList[index] = first->_freeListLink;
			return first;
		}
	}

	static void Deallocate(void* p, size_t n)
	{
		// ����128����һ���ռ��������ͷ�
		if (n > __MAX_BYTES)
		{
			__MallocAllocTemplate<0>::Deallocate(p, n);
		}
		// �ҵ��ڴ�������������е�λ��-->ͷ��
		else
		{
			size_t index = FREELIST_INDEX(n);
			Obj* obj = (Obj*)p;
			obj->_freeListLink = _freeList[index];
			_freeList[index] = obj;
		}
	}
};

template <bool threads, int inst>
typename __DefaultAllocTemplate<threads, inst>::Obj*
__DefaultAllocTemplate<threads, inst>::_freeList[__NFREELISTS] = {0};

// Chunk allocation state.
template <bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_startFree = NULL;
template <bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_endFree = NULL;
template <bool threads, int inst>
size_t __DefaultAllocTemplate<threads, inst>::_heapSize = 0;

// ��װ
template <typename T, class Alloc>
class SimpleAlloc
{
public:
	static T* Allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof(T));
	}

	static T* Allocate()
	{
		return (T*)Alloc::Allocate(sizeof(T));
	}

	static void Deallocate(T* p, size_t n)
	{
		if (0 != n)
			Alloc::Deallocate(p, n * sizeof(T));
	}

	static void Deallocate(T *p)
	{
		Alloc::Deallocate(p, sizeof(T));
	}
};

#ifdef __USE_MALLOC
typedef __MallocAllocTemplate<0> Alloc;
#else
typedef __DefaultAllocTemplate<false, 0> Alloc;
#endif // __USE_MALLOC



// �����ڴ�ص�һ������������������
void TestAllocator()
{
	// ���Ե���һ�������������ڴ�
	cout << " ���Ե���һ�������������ڴ� " << endl;
	char*p1 = SimpleAlloc< char, Alloc>::Allocate(129);
	SimpleAlloc<char, Alloc>::Deallocate(p1, 129);

	// ���Ե��ö��������������ڴ�
	cout << " ���Ե��ö��������������ڴ� " << endl;
	char*p2 = SimpleAlloc< char, Alloc>::Allocate(128);
	char*p3 = SimpleAlloc< char, Alloc>::Allocate(128);
	char*p4 = SimpleAlloc< char, Alloc>::Allocate(128);
	char*p5 = SimpleAlloc< char, Alloc>::Allocate(128);
	SimpleAlloc<char, Alloc>::Deallocate(p2, 128);
	SimpleAlloc<char, Alloc>::Deallocate(p3, 128);
	SimpleAlloc<char, Alloc>::Deallocate(p4, 128);
	SimpleAlloc<char, Alloc>::Deallocate(p5, 128);

	for (int i = 0; i < 21; ++i)
	{
		printf(" ���Ե�%d�η��� \n", i + 1);
		char*p = SimpleAlloc< char, Alloc>::Allocate(128);
	}
}
