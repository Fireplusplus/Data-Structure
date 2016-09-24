#pragma once

#include <iostream>
#include <mutex>
#include <assert.h>

using namespace std;

// 单例：1.全局只有一个实例，2.提供方便获取此实例的接口

// 懒汉模式：直到第一次调用接口才产生实例对象 // 懒加载，与COW思想一致，用到的时候才处理，提高效率
//class Singleton
//{
//public:
//
//	static Singleton* GetInstence()
//	{
//		
//		if (_sInstance == NULL)			// 双重检查，保证高效，避免每次获取实例对象都要加锁
//		{
//			// RAII 资源获取就是初始化：保证在任何情况下，使用对象时先构造对象，最后析构对象
//			lock_guard<mutex> l(lock);	// RAII,new 抛异常后，保证锁被释放
//		//	lock.lock();				// 线程安全
//			if (_sInstance == NULL)		// 实现单例
//			{
//				Singleton* tmp = new Singleton();	// 1.分配空间，2.掉构造函数，3.赋值 //加内存栅栏防止优化顺序
//				MemoryBarrier();
//				_sInstance = tmp;
//			}
//		//	lock.unlock();
//		}
//		
//		return _sInstance;
//	}
//
//	static Delete()
//	{
//		lock_guard<mutex> l(lock);
//		if (_sInstence != NULL)
//		{
//			delete _sInstence;
//			_sInstence = NULL;
//		}
//	}
//
//	~Singleton()
//	{
//		Delete();
//	}
//
//	void Print() const
//	{
//		cout << _data << endl;
//	}
//
//private:
//	// 构造函数限定为私有，限制只能在类里创建对象（全局除外）
//	Singleton()
//		: _data(0)
//	{}
//
//	Singleton(const Singleton&);	// 防拷贝
//  Singleton& opsrator=(const Singleton& s);
//
//	static mutex lock;
//	// 静态私有：通过调用静态成员函数获取对象实例
//	static Singleton* _sInstance;
//	int _data;
//};
//
//mutex Singleton::lock;
//Singleton* Singleton::_sInstance = NULL;

// 饿汉模式：在一开始（进入main函数前）就创建一个实例 // main函数之前只有一个主线程，不存在线程安全问题
// 缺陷：但Singleton构造函数中可能创建了线程，这样就在main函数之前创建了线程，可能引发一些问题，
// 因为main函数之前做的是一些初始化准备工作
//class Singleton
//{
//public:
//
//	static Singleton* GetInstence()
//	{
//		assert(_sInstance != NULL);
//
//		return _sInstance;
//	}
//
//	void Print() const
//	{
//		cout << _data << endl;
//	}
//
//private:
//	Singleton()
//		: _data(0)
//	{}
//
//	Singleton(const Singleton&);	// 防拷贝
//  Singleton& opsrator=(const Singleton& s);
//
//	static Singleton* _sInstance;
//	int _data;
//};
//
//Singleton* Singleton::_sInstance = new Singleton;

// 饿汉模式第二种方法,缺陷同第一种
class Singleton
{
public:

	static Singleton* GetInstence()
	{
		static Singleton tmp;
		return &tmp;
	}

	void Print() const
	{
		cout << _data << endl;
	}

private:
	Singleton()
		: _data(0)
	{}

	Singleton(const Singleton&);	// 防拷贝

	int _data;
};
