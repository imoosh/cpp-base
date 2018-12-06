//
// Created by win on 11/7/18.
//
#include <base/time/timestamp.h>

#include <time.h>
#include <sys/time.h>

TimeStamp::TimeStamp(int32_t sec, int32_t ms)
    : seconds_(sec), microseconds_(ms)
{

}

TimeStamp TimeStamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    return TimeStamp(static_cast<int32_t>(tv.tv_sec), static_cast<int32_t>(tv.tv_usec));
}

string TimeStamp::toString(bool with_microseconds)
{
    char buf[64] = {0};
    struct tm tm_time;
    time_t t = static_cast<time_t>(seconds_);

    localtime_r(&t, &tm_time);

    if (with_microseconds)
    {
        snprintf(buf, sizeof(buf), "%04d%02d%02d%02d%02d%02d%06d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds_);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%04d%02d%02d%02d%02d%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }

    return buf;
}

string TimeStamp::toStampString(bool with_ms)
{
    char buf[32] = {0};

    if (with_ms)
    {
        snprintf(buf, sizeof(buf), "%d.%06d", seconds_, microseconds_);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%d", seconds_);
    }

    return buf;
}

string TimeStamp::toFormattedString(bool with_sec, bool with_ms)
{
    char buf[64] = {0};
    struct tm tm_time;
    time_t t = static_cast<time_t>(seconds_);

    localtime_r(&t, &tm_time);
    if (!with_sec)
    {
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday);
    }
    else
    {
        if (with_ms)
        {
            snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d.%06d",
                     tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                     tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds_);
        }
        else
        {
            snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
                     tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                     tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        }
    }

    return buf;
}

string TimeStamp::toAssignedFotmattedString(const string &format)
{
    char buf[64] = {0};
    struct tm tm_time;
    time_t t = static_cast<time_t>(seconds_);

    localtime_r(&t, &tm_time);
    strftime(buf, sizeof(buf), format.c_str(), &tm_time);

    return string(buf);
}

int32_t TimeStamp::seconds()
{
    return seconds_;
}

int32_t TimeStamp::microseconds()
{
    return microseconds_;
}

int32_t TimeStamp::milliseconds()
{
    return microseconds_ / kMicroSecondsPerMilliSecond;
}