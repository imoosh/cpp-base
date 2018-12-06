//
// Created by win on 11/7/18.
//

#include <base/timer/posix_timer_test.h>

#include <stdio.h>
#include <stdlib.h>

#include <base/time/timestamp.h>

void timerHandler(void *args)
{
    cout << TimeStamp::now().toFormattedString() << " time out." << endl;
    exit(0);
}

int main(int argc, const char *argv[])
{
    PosixTimer *timer = new PosixTimer(timerHandler, nullptr);

    cout << TimeStamp::now().toFormattedString() << " timer start." << endl;
    timer->start(3000);

    pause();

    return 0;
}