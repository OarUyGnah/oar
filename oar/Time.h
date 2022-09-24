#ifndef __OAR_TIME_H__
#define __OAR_TIME_H__
#include "Types.h"
#include "utils/noncopyable.h"
#include <chrono>
#include <time.h>
#include <string>
#include <ostream>

namespace oar {

  //class Time;
  class TimeStamp;
  class TimeZone;
  class Date;
  
  class Time {
  public:
    enum TIMETYPE{
      SECOND,
      MILLSECOND,
      MICROSECOND
    };
    Time() = default;
    //    Time(time_t t);
    //    Time();

    static TimeStamp now();
    static int64_t sinceEpoch(TIMETYPE t = TIMETYPE::SECOND);
    //Time& operator+(time_t t);
    //    Time& operator-(time_t t);
    
  };

  // wait...
  class TimeZone {
  public:
    enum ZONE{
      California = -8,
      NewYork = -5,
      London = 0,
      ShangHai = 8,
      Beijing = 8,
      HongKong = 8,
      ShenZhen = 8,
      Tokyo = 9
    };
    
    TimeZone(ZONE zone = ZONE::Beijing) : _zone(zone) {}
    // + --> east    - --> west
    TimeZone(int zone) : _zone(zone) {}

    int getZone() const {
      return _zone;
    }
    
  private:
    int _zone;
  };
  

  class Date {
  public:
   
    Date();
    Date(time_t t);
    //Date(Time &t);
    Date(TimeStamp ts);
    template<typename T = std::chrono::system_clock::time_point>
    inline Date(T &&tp) {
      time_t t = std::chrono::system_clock::to_time_t(std::forward<T>(tp));
      //      gmtime_r(&t, &_tm);
      localtime_r(&t,&_tm);
    }
    /*
    template<>
    Date(TimeStamp &&ts) {
      time_t t = ts.totime_t();
      localtime_r(&t, &_tm);
    }
    */
    std::string toString() const;

    int getDay() { return _tm.tm_mday; }
    int getYear() { return _tm.tm_year + 1900; }
    int getMonth() { return _tm.tm_mon; }
    int getHour() { return _tm.tm_hour; }
    int getMinute() { return _tm.tm_min; }
    int getSecond() { return _tm.tm_sec; }
    
  private:
    // east +
    //    void UTCToTimezone(struct tm* tm,TimeZone &tz);
    
    struct tm _tm; // localtime
  };


  class TimeStamp /*: Noncopyable*/ {
  public:
    TimeStamp();
    
    TimeStamp(int64_t t) : _microSecondsSinceEpoch(t) {}

    int64_t getmicroEpoch() {
      return _microSecondsSinceEpoch;
    }

    Date toDate() const {
      return Date(static_cast<time_t>(_microSecondsSinceEpoch / 1000000));
    }

    time_t totime_t() const {
      return static_cast<time_t>(_microSecondsSinceEpoch / 1000000);
    }
    
    //    TimeStamp(Date d);
  private:
    int64_t _microSecondsSinceEpoch;
  };

  inline std::ostream& operator<<(std::ostream& os, TimeStamp ts) {
    os << ts.totime_t() << std::endl;
  }
}




#endif
