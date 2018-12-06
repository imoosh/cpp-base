//
// Created by win on 11/7/18.
//
#include <base/timer/posix_timer_test.h>

#include <signal.h>
#include <time.h>

#include <base/types/types.h>

PosixTimer::PosixTimer(PosixTimerCallback func, PosixTimerArgs args)
    : func_(func), args_(args)
{
    struct sigevent se;
    typedef void (*sigev_notify_function_type)(union sigval);

    memset(&se, 0, sizeof(se));
    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_ptr = args;
    se.sigev_notify_function = reinterpret_cast<sigev_notify_function_type>(func);
    se.sigev_notify_attributes = nullptr;

    timer_create(CLOCK_REALTIME, &se, &timer_id_);
}

bool PosixTimer::start(int ms)
{
    struct itimerspec ts;

    memset(&ts, 0, sizeof(ts));
    ts.it_value.tv_sec = ms / 1000;
    ts.it_value.tv_nsec = (ms % 1000) * 1000000;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;

    return !timer_settime(timer_id_, 0, &ts, nullptr);
}

bool PosixTimer::stop()
{
    struct itimerspec ts;

    memset(&ts, 0, sizeof(ts));
    return !timer_settime(timer_id_, 0, &ts, nullptr);
}

bool PosixTimer::isRunning()
{
    struct itimerspec ts;

    memset(&ts, 0, sizeof(ts));
    timer_gettime(timer_id_, &ts);

    return ts.it_value.tv_sec != 0 || ts.it_value.tv_nsec != 0;
}