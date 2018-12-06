//
// Created by win on 11/7/18.
//

#ifndef BASE_POSIX_TIMER_H
#define BASE_POSIX_TIMER_H

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include <base/types/types.h>

typedef void *PosixTimerArgs;
typedef void (*PosixTimerCallback)(void *);

class PosixTimer : boost::noncopyable
{
public:
    PosixTimer(PosixTimerCallback func, PosixTimerArgs args);
    ~PosixTimer();
    bool start(int ms);
    bool stop();
    bool isRunning();

private:
    timer_t     timer_id_;
    PosixTimerCallback func_;
    PosixTimerArgs args_;
};

#endif //BASE_POSIX_TIMER_H