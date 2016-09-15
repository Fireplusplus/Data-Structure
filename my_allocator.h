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
//用于调试追溯的trace log
inline static void __trace_debug(const char* function,
	const char* filename, int line, char* format, ...)
{
#ifdef __DEBUG__
	//输出调用函数的信息
	fprintf(stdout, "【%s:%d】%s", GetFileName(filename).c_str(), line, function);

	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
#endif
}

#define __TRACE_DEBUG(...)  \
__trace_debug(__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__);

typedef void(*HandlerFunc)();

// 一级空间配置器
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
		__TRACE_DEBUG("一级空间配置器分配内存:%d\n", n);

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

// 二级空间配置器
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
	static char* _startFree;	// 内存池的开始
	static char* _endFree;		// 内存池的结束
	static size_t _heapSize;	// 已分配内存的大小

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

		// 内存池有足够nobjs个对象的内存
		if (bytesLeft >= totalBytes)
		{
			__TRACE_DEBUG("内存池有足够nobjs:%d个对象的内存\n", nobjs);

			ret = _startFree;
			_startFree += totalBytes;

			return ret;
		}
		// 内存池只够分配n个对象的内存，不足nobjs
		else if (bytesLeft >= n)
		{
			nobjs = bytesLeft / n;

			__TRACE_DEBUG("只有nobjs:%d个对象的内存\n", nobjs);

			totalBytes = nobjs * n;
			ret = _startFree;
			_startFree += totalBytes;

			return ret;
		}
		// 内存池连一个对象的内存都分配不出来
		else
		{
			// 计算向系统申请的字节数
			size_t bytesToGet = totalBytes * 2 + ROUND_UP(_heapSize >> 4);

			__TRACE_DEBUG("找系统分配%d字节的内存\n", bytesToGet);

			// 处理内存池剩余的字节数
			if (bytesLeft > 0)
			{
				size_t index = FREELIST_INDEX(bytesLeft);
				((Obj*)_startFree)->_freeListLink = _freeList[index];
				_freeList[index] = (Obj*)_startFree;
			}

			_startFree = (char*)malloc(bytesToGet);
			// 分配失败
			if (_startFree == NULL)
			{
				// 在更大的自由链表中去取
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

				// 山穷水尽-->调用一级空间配置器
				_endFree = NULL;	// 此处有可能抛出异常，外界可能捕获异常，保证_startFree和_endFree相等
				_startFree = (char*)__MallocAllocTemplate<0>::Allocate(bytesToGet);
			}

			// malloc或一级空间配置器分配成功
			_endFree = _startFree + bytesToGet;
			_heapSize = bytesToGet;
			return ChunkAlloc(n, nobjs);
		}
	}

	static void* Refill(size_t n)
	{
		// 分配nobjs个对象的大小
		size_t nobjs = 20;
		char* chunk = ChunkAlloc(n, nobjs);
		if (n == 1)
			return chunk;

		__TRACE_DEBUG("挂内存块到自由链表nobjs:%d\n", nobjs);
		// 挂内存块
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
		__TRACE_DEBUG("二级空间配置器分配内存:%d\n", n);

		// 大于128调用一级空间配置器申请
		if (n > __MAX_BYTES)
		{
			return __MallocAllocTemplate<inst>::Allocate(n);
		}

		size_t index = FREELIST_INDEX(n);
		// 自由链表对应位置没有-->填充
		if (_freeList[index] == NULL)
		{
			return Refill(ROUND_UP(n));
		}
		// 有-->头删
		else
		{
			__TRACE_DEBUG("到自由链表中去取内存块:%d\n", index);

			Obj* first = _freeList[index];
			_freeList[index] = first->_freeListLink;
			return first;
		}
	}

	static void Deallocate(void* p, size_t n)
	{
		// 大于128调用一级空间配置器释放
		if (n > __MAX_BYTES)
		{
			__MallocAllocTemplate<0>::Deallocate(p, n);
		}
		// 找到内存块在自由链表中的位置-->头插
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

// 封装
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



// 测试内存池的一级、二级配置器功能
void TestAllocator()
{
	// 测试调用一级配置器分配内存
	cout << " 测试调用一级配置器分配内存 " << endl;
	char*p1 = SimpleAlloc< char, Alloc>::Allocate(129);
	SimpleAlloc<char, Alloc>::Deallocate(p1, 129);

	// 测试调用二级配置器分配内存
	cout << " 测试调用二级配置器分配内存 " << endl;
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
		printf(" 测试第%d次分配 \n", i + 1);
		char*p = SimpleAlloc< char, Alloc>::Allocate(128);
	}
}
