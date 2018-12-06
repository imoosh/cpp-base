//
// Created by win on 11/13/18.
//

#include <base/thread/thread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

class ThreadTest
{
public:
    ThreadTest()
        : thread_(new Thread(boost::bind(&ThreadTest::ThreadFunc, this)))
    {
        thread_->start();
    }

    explicit ThreadTest(int num_threads)
    {
        for (int i = 0; i < num_threads; i++)
        {
            threads_.push_back(new Thread(boost::bind(&ThreadTest::ThreadFunc, this)));
        }

        for_each(threads_.begin(), threads_.end(), boost::bind(&Thread::start, _1));
    }

    void ThreadFunc(void)
    {
        while (true)
        {
            printf("ThreadTest...\n");
            sleep(1);
        }
    }

private:
    Thread *thread_;
    boost::ptr_vector<Thread> threads_;
};

void ThreadFunc()
{
    while (true)
    {
        printf("in ThreadFunc...\n");
        sleep(1);
    }
}

void test1()
{
    Thread *t = new Thread(ThreadFunc, "Thread01");
    t->start();
}

void test2()
{
#if 1
    ThreadTest t;
#else
    ThreadTest t();
    t();
#endif
}

void test3()
{
    ThreadTest t(5);
}

int main(int argc, const char *argv[])
{
    test2();
    pause();

    return 0;
}

ThreadTest t()
{
    ThreadTest x;
    return x;
}