//
// Created by win on 11/8/18.
//

#include <base/thread/thread.h>

#include <pthread.h>

#include <boost/bind.hpp>

#include <base/thread/mutexlock.h>
#include <base/thread/condvariable.h>

#define LOG_SYSERR printf

Thread::Thread(const Thread::ThreadFunc &func, string name)
    : tid_(0), name_(name), func_(func)
{

}

Thread::~Thread()
{
    detach();
}

void* Thread::runInThread(void *params)
{
    Thread *thread_params = static_cast<Thread *>(params);

    thread_params->func_();

    return nullptr;
}

void Thread::start()
{
    PTHREAD_CHECK(pthread_create(&tid_, nullptr, runInThread, this));
}

void Thread::join()
{
    PTHREAD_CHECK(pthread_join(tid_, nullptr));
}

void Thread::detach()
{
    if (tid_)
    {
        PTHREAD_CHECK(pthread_detach(tid_));
    }
}