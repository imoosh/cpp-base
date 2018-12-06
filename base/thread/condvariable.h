//
// Created by win on 11/8/18.
//

#ifndef CODEBASE_CONDVARIABLE_H
#define CODEBASE_CONDVARIABLE_H

#include <pthread.h>

#include <boost/noncopyable.hpp>

#include <base/thread/mutexlock.h>

class CondVariable : boost::noncopyable
{
public:
    explicit CondVariable(MutexLock &lock)
        : lock_(lock)
    {
        PTHREAD_CHECK(pthread_cond_init(&cond_, nullptr));
    }

    ~CondVariable()
    {
        PTHREAD_CHECK(pthread_cond_destroy(&cond_));
    }

    void wait()
    {
        PTHREAD_CHECK(pthread_cond_wait(&cond_, lock_.getLock()));
    }

    void notify()
    {
        PTHREAD_CHECK(pthread_cond_signal(&cond_));
    }

    void notifyAll()
    {
        PTHREAD_CHECK(pthread_cond_broadcast(&cond_));
    }

    bool waitForSeconds(int seconds);

private:
    MutexLock &lock_;
    pthread_cond_t cond_;
};

#endif //CODEBASE_CONDVARIABLE_H
