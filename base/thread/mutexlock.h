//
// Created by win on 11/8/18.
//

#ifndef BASE_MUTEXLOCK_H
#define BASE_MUTEXLOCK_H

#include <pthread.h>

#include <base/types/types.h>

#include <boost/noncopyable.hpp>

#ifdef CHECK_THREAD_RETURN_VALUE
#define PTHREAD_CHECK(ret) ({   \
    __typeof__(ret) errnum = (ret); \
    if(UNLIKELY(errnum)) ASSERT_PERROR(errnum); \
})
#else // !CHECK_THREAD_RETURN_VALUE
#define PTHREAD_CHECK(ret) ({   \
    __typeof__(ret) errnum = (ret); \
    if(UNLIKELY(errnum)) ASSERT(errnum); \
})
#endif  // CHECK_THREAD_RETURN_VALUE

class MutexLock : boost::noncopyable
{
public:
    MutexLock()
    {
        PTHREAD_CHECK(pthread_mutex_init(&lock_, nullptr));
    }

    ~MutexLock()
    {
        PTHREAD_CHECK(pthread_mutex_destroy(&lock_));
    }

    void lock()
    {
        PTHREAD_CHECK(pthread_mutex_lock(&lock_));
    }

    void unlock()
    {
        PTHREAD_CHECK(pthread_mutex_unlock(&lock_));
    }

    pthread_mutex_t *getLock()
    {
        return &lock_;
    }

private:
    pthread_mutex_t lock_;
};

class ScopedLock
{
public:
    explicit ScopedLock(MutexLock &lock)
        : lock_(lock)
    {
        lock_.lock();
    }

    ~ScopedLock()
    {
        lock_.unlock();
    }

private:
    MutexLock &lock_;
};

#endif //BASE_MUTEXLOCK_H
