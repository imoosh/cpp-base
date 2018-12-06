//
// Created by root on 12/5/18.
//

#ifndef CODEBASE_SINGLETON_H
#define CODEBASE_SINGLETON_H

#include <base/thread/mutexlock.h>

#include <base/types/types.h>

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

// 参考https://zhuanlan.zhihu.com/p/37469260

// 懒汉单例模式之释放单例对象：使用嵌套类
class SingletonVer1 : boost::noncopyable
{
public:
    static SingletonVer1* getInstance()
    {
        if (!instance_)
        {
            instance_ = new SingletonVer1();
        }

        return instance_;
    }


private:
    static SingletonVer1 *instance_;

    SingletonVer1() = default;
    ~SingletonVer1() = default;

    class Deleter
    {
    public:
        ~Deleter()
        {
            if (!instance_)
            {
                delete instance_;
            }
        }
    };
    static Deleter deleter_;
};

SingletonVer1 *SingletonVer1::instance_ = nullptr;

////////////////////////////////////////////////////


// // 懒汉单例模式之释放单例对象：使用智能指针
class SingletonVer2 : boost::noncopyable
{
public:
    static SingletonVer2 *getInstance()
    {
        if (!instance_)
        {
            instance_ = new SingletonVer2;
            ptr_instance_.reset(instance_);
        }

        return ptr_instance_.get();
    }

    SingletonVer2() = default;

    ~SingletonVer2()
    {
        if (!instance_)
        {
            delete(instance_);
        }
    }

private:
    static SingletonVer2 *instance_;
    static boost::scoped_ptr<SingletonVer2> ptr_instance_;
};

SingletonVer2* SingletonVer2::instance_ = nullptr;
boost::scoped_ptr<SingletonVer2> SingletonVer2::ptr_instance_;

////////////////////////////////////////////////////


// 懒汉单例模式之线程安全
class SingletonVer3 : boost::noncopyable
{
public:
    static SingletonVer3* getInstance()
    {
        if (!instance_)
        {
            ScopedLock lock(lock_);
            {
                if (!instance_)
                {
                    instance_ = new SingletonVer3();
                }
            }
        }

        return instance_;
    }
private:

    static MutexLock lock_;
    static SingletonVer3 *instance_;

    SingletonVer3() = default;
    ~SingletonVer3() = default;
};

MutexLock SingletonVer3::lock_;
SingletonVer3* SingletonVer3::instance_ = nullptr;

////////////////////////////////////////////////////

// 懒汉单例模式之局部静态变量（Meyers Singleton, best of all！）
class SingletonVer4 : boost::noncopyable
{
public:
    static SingletonVer4& getInstance()
    {
        // for c11 safety, https://zhuanlan.zhihu.com/p/37469260
        static SingletonVer4 instance;
        return instance;
    }

//    static SingletonVer4* getInstance()
//    {
//        // for c11 safety, https://zhuanlan.zhihu.com/p/37469260
//        static SingletonVer4 instance;
//        return &instance;
//    }

private:
    SingletonVer4() = default;
    ~SingletonVer4() = default;
};

////////////////////////////////////////////////////

// 饿汉单例模式:
class SingletonVer5 : boost::noncopyable
{
public:
    SingletonVer5* getInstance()
    {
        return &instance_;
    }

//    SingletonVer5& getInstance()
//    {
//        return instance_;
//    }

private:
    SingletonVer5() = default;
    ~SingletonVer5() = default;

    static SingletonVer5 instance_;
};

// 虽然保证在main()之前初始化好，但是不保证SingletonVer5 *getInstance()与
// SingletonVer5 SingletonVer5::instance_的初始化顺序，如果在后者初始化好之前
// 调用getInstance(被其它文件中全局变量调用初始化)，则会获取到空对象。
SingletonVer5 SingletonVer5::instance_;

////////////////////////////////////////////////////

// 使用pthread_once
class SingletonVer6
{

};

////////////////////////////////////////////////////

// 懒汉单例模式之局部静态变量（Meyers Singleton, best of all）
template <typename T>
class Singleton : boost::noncopyable
{
private:
    Singleton() = default;
    ~Singleton() = default;

public:
    // 1.若T是同一类型的，先后传不同的参数列表，会得到不同的实例对象。
    //   若T不是同一类型的，则会得到各自类的实例对象
    // 2.若无参数传递，需要找到T(const T&)拷贝构造函数，如果被标记delete，见类AsyncLogging
    //   则编译会报错，所以下面重载了一个无参数的getInstance函数。
    template <typename ... Args>
    static T* getInstance(Args&& ... args)
    {
        static T t(args...);
        return &t;
    }

    static T* getInstance()
    {
        static T t;
        return &t;
    }
};

#endif //CODEBASE_SINGLETON_H
