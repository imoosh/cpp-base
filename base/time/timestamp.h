//
// Created by win on 11/7/18.
//

#ifndef BASE_TIMESTAMP_H
#define BASE_TIMESTAMP_H

#include <stdint.h>

#include <boost/noncopyable.hpp>

#include <base/types/types.h>

class TimeStamp
{
public:
    int32_t seconds();
    int32_t microseconds();
    int32_t milliseconds();

    string toString(bool with_microseconds=false);
    string toStampString(bool with_microseconds=false);
    string toFormattedString(bool with_sec=true, bool with_ms=true);
    string toAssignedFotmattedString(const string& format);
    static TimeStamp now();

private:
    TimeStamp(int32_t sec=0, int32_t ms=0);

    int32_t seconds_;
    int32_t microseconds_;

    static const int kMilliSecondsPerSecond = 1000;
    static const int kMicroSecondsPerSecond = 1000000;
    static const int kMicroSecondsPerMilliSecond = 1000;
};

#endif //BASE_TIMESTAMP_H
