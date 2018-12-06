//
// Created by win on 11/8/18.
//

#include <base/thread/condvariable.h>

#include <time.h>

#include <base/types/types.h>

bool CondVariable::waitForSeconds(int seconds)
{
    struct timespec abstime;
    const static int64_t kNanoSecondsPerSecond = 1000000000;
    int64_t nonoseconds = seconds * kNanoSecondsPerSecond;

    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nonoseconds) / kNanoSecondsPerSecond);
    abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + kNanoSecondsPerSecond) % kNanoSecondsPerSecond);

    return ETIMEDOUT == pthread_cond_timedwait(&cond_, lock_.getLock(), &abstime);
}