#pragma once

#include <iostream>
#include <mutex>
#include <assert.h>

using namespace std;

// ������1.ȫ��ֻ��һ��ʵ����2.�ṩ�����ȡ��ʵ���Ľӿ�

// ����ģʽ��ֱ����һ�ε��ýӿڲŲ���ʵ������ // �����أ���COW˼��һ�£��õ���ʱ��Ŵ������Ч��
//class Singleton
//{
//public:
//
//	static Singleton* GetInstence()
//	{
//		
//		if (_sInstance == NULL)			// ˫�ؼ�飬��֤��Ч������ÿ�λ�ȡʵ������Ҫ����
//		{
//			// RAII ��Դ��ȡ���ǳ�ʼ������֤���κ�����£�ʹ�ö���ʱ�ȹ�����������������
//			lock_guard<mutex> l(lock);	// RAII,new ���쳣�󣬱�֤�����ͷ�
//		//	lock.lock();				// �̰߳�ȫ
//			if (_sInstance == NULL)		// ʵ�ֵ���
//			{
//				Singleton* tmp = new Singleton();	// 1.����ռ䣬2.�����캯����3.��ֵ //���ڴ�դ����ֹ�Ż�˳��
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
//	// ���캯���޶�Ϊ˽�У�����ֻ�������ﴴ������ȫ�ֳ��⣩
//	Singleton()
//		: _data(0)
//	{}
//
//	Singleton(const Singleton&);	// ������
//  Singleton& opsrator=(const Singleton& s);
//
//	static mutex lock;
//	// ��̬˽�У�ͨ�����þ�̬��Ա������ȡ����ʵ��
//	static Singleton* _sInstance;
//	int _data;
//};
//
//mutex Singleton::lock;
//Singleton* Singleton::_sInstance = NULL;

// ����ģʽ����һ��ʼ������main����ǰ���ʹ���һ��ʵ�� // main����֮ǰֻ��һ�����̣߳��������̰߳�ȫ����
// ȱ�ݣ���Singleton���캯���п��ܴ������̣߳���������main����֮ǰ�������̣߳���������һЩ���⣬
// ��Ϊmain����֮ǰ������һЩ��ʼ��׼������
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
//	Singleton(const Singleton&);	// ������
//  Singleton& opsrator=(const Singleton& s);
//
//	static Singleton* _sInstance;
//	int _data;
//};
//
//Singleton* Singleton::_sInstance = new Singleton;

// ����ģʽ�ڶ��ַ���,ȱ��ͬ��һ��
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

	Singleton(const Singleton&);	// ������

	int _data;
};
