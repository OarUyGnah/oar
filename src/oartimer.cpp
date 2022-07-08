#include "oartimer.h"

namespace oar {

    //模板方法放上侧，否则出现模板专用化问题
    template<>
    int timer::second<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return ts.second;
    }
    template<>
    int timer::second<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts.second;
    }

    template<>
    int timer::minute<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return ts.minute;
    }
    template<>
    int timer::minute<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts.minute;
    }

    template<>
    int timer::hour<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return ts.hour;
    }

    template<>
    int timer::hour<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts.hour;
    }

    template<>
    int timer::hourFormat12<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        int res;
        return hoursFormatTo12(ts.hour, res);
    }

    template<>
    int timer::hourFormat12<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        int res;
        return hoursFormatTo12(ts.hour, res);
    }

    template<>
    int timer::weekday<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return ts.weekday;
    }

    template<>
    int timer::weekday<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts.weekday;
    }

    template<>
    int timer::day<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return ts.day;
    }

    template<>
    int timer::day<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts.day;
    }

    template<>
    int timer::month<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return ts.month;
    }

    template<>
    int timer::month<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts.month;
    }

    template<>
    int timer::year<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return ts.year;
    }

    template<>
    int timer::year<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts.year;
    }
    
    /*******************   to_time_str   *******************/
    //不仅要考虑左右值还要考虑值传递

    const char* timer::__Return_Time_Str(time_t tt) {
        *timestr = std::string(ctime(&tt));
        return timestr->c_str();
    }
    template<>
    const char* timer::__To_Time_Str<timer::_Tp>(timer::_Tp timepoint)
    {
        std::cout << "_Tp" << std::endl;
        //_Tp tmp = std::move(timepoint);
        time_t tt = _Clock::to_time_t(timepoint);
        *timestr = std::string(ctime(&tt));
        return timestr->c_str();
    }
    template<>
    const char* timer::__To_Time_Str<const timer::_Tp&>(const timer::_Tp& timepoint)
    {
        std::cout << "_Tp&" << std::endl;
        time_t tt = _Clock::to_time_t(timepoint);
        *timestr = std::string(ctime(&tt));
        return timestr->c_str();
    }
    template<>
    const char* timer::__To_Time_Str<timer::_Tp&&>(timer::_Tp&& timepoint)
    {
        std::cout << "_Tp&&" << std::endl;
        _Tp tmp = std::move(timepoint);
        time_t tt = _Clock::to_time_t(tmp);
        *timestr = std::string(ctime(&tt));
        return timestr->c_str();
    }
    template<>
    const char* timer::__To_Time_Str<time_t>(time_t t)
    {
        std::cout << "time_t" << std::endl;
        *timestr = std::string(ctime(&t));
        return timestr->c_str();
    }
    const char* timer::to_time_str(_Tp& timepoint)
    {
        std::cout << "to_time_str _Tp&" << std::endl;
        return __To_Time_Str(std::forward<_Tp&>(timepoint));
    }
    const char* timer::to_time_str(_Tp&& timepoint)
    {
        std::cout << "to_time_str _Tp&&" << std::endl;
        return __To_Time_Str(std::forward<_Tp&&>(timepoint));
    }
    const char* timer::to_time_str(time_t t) {
        return __To_Time_Str(t);
    } 
    /*******************   to_time_str   *******************/

    /*******************   parseTime   *******************/
    //template<class T>
    void timer::__Parse_Time(_Tp timepoint)
    {
        // break the given time_t into time components
            // this is a more compact version of the C library localtime function
            // note that year is offset from 1970 !!!
        int year;
        int month, monthLength;
        time_t time;
        unsigned long days;

        time = _Clock::to_time_t(timepoint);
        current = _Clock::now();
        ts.second = time % 60;
        time /= 60;
        ts.minute = time % 60;
        time /= 60;
        ts.hour = time % 24 + 8;//东八区 要+8
        time /= 24;
        ts.weekday = ((time + 4) % 7);
        year = 0;
        days = 0;
        while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
            year++;
        }
        ts.year = year + 1970;
        days -= LEAP_YEAR(year) ? 366 : 365;
        time -= days; // now it is days in this year, starting at 0

        days = 0;
        month = 0;
        monthLength = 0;
        for (month = 0; month < 12; month++) {
            if (month == 1) { // february
                if (LEAP_YEAR(ts.year)) {
                    monthLength = 29;
                }
                else {
                    monthLength = 28;
                }
            }
            else {
                monthLength = monthDays[month];
            }

            if (time >= monthLength) {
                time -= monthLength;
            }
            else {
                break;
            }
        }
        ts.month = month + 1;  // jan is month 1  
        ts.day = time + 1;     // day of month
    }

    void timer::parseTime(_Tp& timepoint)
    {
        __Parse_Time(std::forward<_Tp&>(timepoint));
    }

    void timer::parseTime(_Tp&& timepoint)
    {
        __Parse_Time(std::forward<_Tp&&>(timepoint));
    }

    /*void timer::parseTime(time_t t) {
        __Parse_Time(t);
    }*/

    /*******************   parseTime   *******************/
    

    

    time_t timer::__To_Time_T(_Tp timepoint)
    {
        return _Clock::to_time_t(/*std::forward<_Tp&>*/(timepoint));//rvalue waiting...
    }

    

    void timer::printTimeStruct()
    {
        std::cout << "year : " << (int)ts.year << std::endl;
        std::cout << "month : " << (int)ts.month << std::endl;
        std::cout << "day : " << (int)ts.day << std::endl;
        std::cout << "hour : " << (int)ts.hour << std::endl;
        std::cout << "minute : " << (int)ts.minute << std::endl;
        std::cout << "second : " << (int)ts.second << std::endl;
        std::cout << "weekday : " << (int)ts.weekday << std::endl;
    }

    timer::timer()
    {
        timestr = new std::string;
        start = std::chrono::system_clock::now();
        current = start;
        tt = _Clock::to_time_t(start);
        parseTime();
    }

    timer::~timer()
    {
        delete timestr;
    }

    void timer::printCurrentTime(const char* format, std::ostream& os)
    {
        os << to_time_str(getCurrTp()) << '\n';
    }

    timer::_Tp& timer::getStartTp()
    {
        return start;
    }

    timer::_Tp& timer::getCurrTp()
    {
        return current;
    }

    
    time_t timer::to_time_t(_Tp& timepoint) {
        return __To_Time_T(timepoint);
    }
    time_t timer::to_time_t(_Tp&& timepoint) {
        //std::cout << "&&" << std::endl;
        return __To_Time_T(timepoint);
    }
    timer::_Tp timer::from_time_t(time_t time)
    {
        return _Clock::from_time_t(time);
    }

    timer::_Tp timer::now()
    {
        return _Clock::now();
    }

    int timer::second()
    {
        parseTime();
        return ts.second;
    }

    int timer::minute()
    {
        parseTime();
        return ts.minute;
    }

    int timer::hour()
    {
        parseTime();
        return ts.hour;
    }

    int timer::hourFormat12()
    {
        parseTime();
        int res;
        return hoursFormatTo12(ts.hour, res);
    }

    int timer::weekday()
    {
        parseTime();
        return ts.weekday;
    }

    int timer::day()
    {
        parseTime();
        return ts.day;
    }

    int timer::month()
    {
        parseTime();
        return ts.month;
    }

    int timer::year()
    {
        parseTime();
        return ts.year;
    }

    bool timer::isAM()
    {

        return !isPM();

    }

    bool timer::isPM()
    {
        return (hour(_Clock::now())) >= 12;
    }



}