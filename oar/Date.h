#ifndef __OAR_DATE_H__
#define __OAR_DATE_H__

#include "./utils/noncopyable.h"

struct tm;

namespace oar {

  class Date : Noncopyable {
  public:
    static constexpr int __daysPerWeek = 7;
    static constexpr int __julianDayOf1970_01_01;

    struct ymd {
      int _year;
      int _month;
      int _day;
    };

    
    Date(time_t t);
    //Date(const struct tm&);
    //Date(int julianDayNum);
    //Date(int year,int month,int day);

    std::string toString() const;
    bool isValid() const;

    int year() const { return _ymd._year; }
    int month() const { return _ymd._month; }
    int day() const { return _ymd._day; }
    
  private:
    ymd _ymd;
    
  };

}



#endif
