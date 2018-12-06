//
// Created by win on 11/8/18.
//

#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include <pthread.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include <base/types/types.h>
#include <base/thread/mutexlock.h>
#include <base/thread/condvariable.h>

class Thread
{
public:

    typedef boost::function<void(void)> ThreadFunc;

    Thread(const ThreadFunc &func, string name="");
    ~Thread();

    void start();
    void join();
    void detach();

    pthread_t getThreadId()
    {
        return tid_;
    }

    string getThreadName()
    {
        return name_;
    }

private:
    static void *runInThread(void *params);

    pthread_t tid_;
    string name_;
    ThreadFunc func_;
};

#endif //BASE_THREAD_H
