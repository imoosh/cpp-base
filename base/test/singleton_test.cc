//
// Created by root on 12/5/18.
//

#include <base/singleton/singleton.h>

#include <boost/noncopyable.hpp>

class SingletonTest1 : boost::noncopyable
{
public:
    int a;
    int b;

    SingletonTest1() = default;
    SingletonTest1(int x) : a(x), b(0){}
    SingletonTest1(int x, int y) : a(x), b(y){}

    ~SingletonTest1() = default;
    SingletonTest1(const SingletonTest1&);
    SingletonTest1& operator=(const SingletonTest1&);
};

class SingletonTest2 : boost::noncopyable
{
public:
    int m;
    int n;

    SingletonTest2() = default;
    SingletonTest2(int x) : m(x), n(0){}
    SingletonTest2(int x, int y) : m(x), n(y){}

    ~SingletonTest2() = default;
    SingletonTest2(const SingletonTest2&);
    SingletonTest2& operator=(const SingletonTest2&);
};

int main(int argc, const char *argv[])
{
    SingletonTest1* t = Singleton<SingletonTest1>::getInstance();
    cout << t->a << ", " << t->b << endl;

    SingletonTest1* s = Singleton<SingletonTest1>::getInstance(10);
    cout << s->a << ", " << s->b << endl;

    SingletonTest1* r = Singleton<SingletonTest1>::getInstance(100, 200);
    cout << r->a << ", " << r->b << endl;

    return 0;
}