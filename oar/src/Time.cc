#include "oar/Time.h"
#include <chrono>
#include <sstream>
#include <sys/time.h>

namespace oar {

TimeStamp Time::now()
{
    return TimeStamp();
}

int64_t Time::sinceEpoch(TIMETYPE t)
{
    TimeStamp ts;
    switch (t) {
    case SECOND:
        break;
    case MILLSECOND:
        return static_cast<int64_t>(ts.getmicroEpoch() / 1000);
    case MICROSECOND:
        return ts.getmicroEpoch();
    default:
        break;
    }
    return static_cast<int64_t>(ts.totime_t());
}

TimeStamp::TimeStamp()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    _microSecondsSinceEpoch = tv.tv_sec * 1000000 + tv.tv_usec;
}

Date::Date()
{
    time_t t = time(nullptr);
    //    gmtime_r(&t, &_tm);
    localtime_r(&t, &_tm);
}
Date::Date(time_t t)
{
    //    gmtime_r(&t, &_tm);
    localtime_r(&t, &_tm);
}

Date::Date(TimeStamp ts)
{
    time_t t = ts.totime_t();
    localtime_r(&t, &_tm);
}

std::string Date::toString() const
{
    char time[32];
    strftime(time, 32, "%Y-%m-%d %2H:%2M:%2S", &_tm);
    return time;
}

/*
  void UTCToTimezone(struct tm* tm,int timezone) {

  }
*/
}
