#include "oartimer.h"

namespace oar {

    //模板方法放上侧，否则出现模板专用化问题
    template<>
    int TimerImpl::second<TimerImpl::_Tp>(TimerImpl::_Tp t) {
        parseTime(t);
        return ts->second;
    }
    template<>
    int TimerImpl::second<time_t>(time_t t) {
        //parseTime(_Clock::from_time_t(t));
        parseTime(t);
        return ts->second;
    }

    template<>
    int TimerImpl::minute<TimerImpl::_Tp>(TimerImpl::_Tp t) {
        parseTime(t);
        return ts->minute;
    }
    template<>
    int TimerImpl::minute<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts->minute;
    }

    template<>
    int TimerImpl::hour<TimerImpl::_Tp>(TimerImpl::_Tp t) {
        parseTime(t);
        return ts->hour;
    }

    template<>
    int TimerImpl::hour<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts->hour;
    }

    template<>
    int TimerImpl::hourFormat12<TimerImpl::_Tp>(TimerImpl::_Tp t) {
        parseTime(t);
        int res;
        return hoursFormatTo12(ts->hour, res);
    }

    template<>
    int TimerImpl::hourFormat12<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        int res;
        return hoursFormatTo12(ts->hour, res);
    }

    template<>
    int TimerImpl::weekday<TimerImpl::_Tp>(TimerImpl::_Tp t) {
        parseTime(t);
        return ts->weekday;
    }

    template<>
    int TimerImpl::weekday<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts->weekday;
    }

    template<>
    int TimerImpl::day<TimerImpl::_Tp>(TimerImpl::_Tp t) {
        parseTime(t);
        return ts->day;
    }

    template<>
    int TimerImpl::day<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts->day;
    }

    template<>
    int TimerImpl::month<TimerImpl::_Tp>(TimerImpl::_Tp t) {
        parseTime(t);
        return ts->month;
    }

    template<>
    int TimerImpl::month<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts->month;
    }

    template<>
    int TimerImpl::year<TimerImpl::_Tp>(TimerImpl::_Tp t) {
        parseTime(t);
        return ts->year;
    }

    template<>
    int TimerImpl::year<time_t>(time_t t) {
        parseTime(_Clock::from_time_t(t));
        return ts->year;
    }

    /*******************   to_time_str   *******************/

    const char* TimerImpl::__To_Time_Str(time_t tt) {
        *timestr = std::string(ctime(&tt));
        return timestr->c_str();
    }
    template<>
    const char* TimerImpl::__To_Time_Str_Aux<TimerImpl::_Tp>(TimerImpl::_Tp timepoint)//only work
    {
        //std::cout << "_Tp" << std::endl;
        //_Tp tmp = std::move(timepoint);
        return __To_Time_Str(_Clock::to_time_t(timepoint));
    }
    template<>
    const char* TimerImpl::__To_Time_Str_Aux<const TimerImpl::_Tp&>(const TimerImpl::_Tp& timepoint)
    {
        //std::cout << "_Tp&" << std::endl;
        return __To_Time_Str(_Clock::to_time_t(timepoint));
    }
    template<>
    const char* TimerImpl::__To_Time_Str_Aux<TimerImpl::_Tp&&>(TimerImpl::_Tp&& timepoint)
    {
        //std::cout << "_Tp&&" << std::endl;
        return __To_Time_Str(_Clock::to_time_t(timepoint));
    }
    template<>
    const char* TimerImpl::__To_Time_Str_Aux<time_t>(time_t t)
    {
        //std::cout << "time_t" << std::endl;
        return __To_Time_Str(t);
    }
    const char* TimerImpl::to_time_str(const _Tp& timepoint)
    {
        //std::cout << "to_time_str _Tp&" << std::endl;
        return __To_Time_Str_Aux(std::forward<const _Tp>(timepoint));
    }
    const char* TimerImpl::to_time_str(_Tp&& timepoint)
    {
        //std::cout << "to_time_str _Tp&& " << std::endl;
        return __To_Time_Str_Aux(std::forward<_Tp>(timepoint));
    }
    const char* TimerImpl::to_time_str(time_t t) {
        return __To_Time_Str_Aux(t);
    }
    /*******************   to_time_str   *******************/

    /*******************   parseTime   *******************/
    void TimerImpl::__Parse_Time(time_t t)
    {
        // break the given time_t into time components
            // this is a more compact version of the C library localtime function
            // note that year is offset from 1970 !!!
        int year;
        int month, monthLength;
        time_t time;
        unsigned long days;

        time = t;
        current = _Clock::now();
        ts->second = time % 60;
        time /= 60;
        ts->minute = time % 60;
        time /= 60;
        ts->hour = time % 24 + 8;//东八区 要+8
        time /= 24;
        ts->weekday = ((time + 4) % 7);
        year = 0;
        days = 0;
        while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
            year++;
        }
        ts->year = year + 1970;
        days -= LEAP_YEAR(year) ? 366 : 365;
        time -= days; // now it is days in this year, starting at 0

        days = 0;
        month = 0;
        monthLength = 0;
        for (month = 0; month < 12; month++) {
            if (month == 1) { // february
                if (LEAP_YEAR(ts->year)) {
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
        ts->month = month + 1;  // jan is month 1  
        ts->day = time + 1;     // day of month
    }


    template<>
    void TimerImpl::__Parse_Time_Aux<TimerImpl::_Tp>(TimerImpl::_Tp timepoint)
    {
        __Parse_Time(_Clock::to_time_t(timepoint));
    }
    template<>
    void TimerImpl::__Parse_Time_Aux<time_t>(time_t t) {
        __Parse_Time(t);
    }
    
    void TimerImpl::parseTime(const TimerImpl::_Tp& timepoint)
    {
        __Parse_Time_Aux(std::forward<const _Tp&>(timepoint));
    }

    void TimerImpl::parseTime(_Tp&& timepoint)
    {
        __Parse_Time_Aux(std::forward<_Tp&&>(timepoint));
    }

    void TimerImpl::parseTime(time_t t) {
        __Parse_Time_Aux(t);
    }

    /*******************   parseTime   *******************/




    time_t TimerImpl::__To_Time_T(_Tp timepoint)
    {
        return _Clock::to_time_t(/*std::forward<_Tp&>*/(timepoint));//rvalue waiting...
    }



    /*void TimerImpl::printTimeStruct()
    {
        std::cout << "year : " << (int)ts->year << std::endl;
        std::cout << "month : " << (int)ts->month << std::endl;
        std::cout << "day : " << (int)ts->day << std::endl;
        std::cout << "hour : " << (int)ts->hour << std::endl;
        std::cout << "minute : " << (int)ts->minute << std::endl;
        std::cout << "second : " << (int)ts->second << std::endl;
        std::cout << "weekday : " << (int)ts->weekday << std::endl;
    }*/

    void TimerImpl::init()
    {
        timestr = new std::string;
        ts = new time_struct;
        current = start;
        parseTime();
    }
    
    TimerImpl::TimerImpl()
    {
        start = std::chrono::system_clock::now();
        init();
        //tt = _Clock::to_time_t(start);
    } 

    TimerImpl::TimerImpl(time_t t)
    {
        start = from_time_t(t);
        init();
    }

    TimerImpl::TimerImpl(const _Tp& t)
    {
        start = t;
        init();
    }

    TimerImpl::TimerImpl(_Tp&& t)
    {
        start = std::move(t);
        init();
    }

    TimerImpl::~TimerImpl()
    {
        delete timestr;
    }

    void TimerImpl::printCurrentTime(const char* format, std::ostream& os)
    {
        os << to_time_str(getCurrTp()) << '\n';
    }

    TimerImpl::_Tp& TimerImpl::getStartTp()
    {
        return start;
    }

    TimerImpl::_Tp& TimerImpl::getCurrTp()
    {
        return current;
    }


    time_t TimerImpl::to_time_t(const _Tp& timepoint) {
        return __To_Time_T(timepoint);
    }
    time_t TimerImpl::to_time_t(_Tp&& timepoint) {
        //std::cout << "&&" << std::endl;
        return __To_Time_T(timepoint);
    }
    TimerImpl::_Tp TimerImpl::from_time_t(time_t time)
    {
        return _Clock::from_time_t(time);
    }

    TimerImpl::_Tp TimerImpl::now()
    {
        return _Clock::now();
    }

    int TimerImpl::second()
    {
        parseTime();
        return ts->second;
    }

    int TimerImpl::minute()
    {
        parseTime();
        return ts->minute;
    }

    int TimerImpl::hour()
    {
        parseTime();
        return ts->hour;
    }

    int TimerImpl::hourFormat12()
    {
        parseTime();
        int res;
        return hoursFormatTo12(ts->hour, res);
    }

    int TimerImpl::weekday()
    {
        parseTime();
        return ts->weekday;
    }

    int TimerImpl::day()
    {
        parseTime();
        return ts->day;
    }

    int TimerImpl::month()
    {
        parseTime();
        return ts->month;
    }

    int TimerImpl::year()
    {
        parseTime();
        return ts->year;
    }

    bool TimerImpl::isAM()
    {

        return !isPM();

    }

    bool TimerImpl::isPM()
    {
        return (hour(_Clock::now())) >= 12;
    }



    timer::timer()
    {
        _pImpl = new TimerImpl;
    }

    timer::timer(const _Tp& t)
    {
        _pImpl = new TimerImpl(t);
    }

    timer::timer(_Tp&& t)
    {
        _pImpl = new TimerImpl(std::forward<_Tp>(t));
    }

    timer::timer(time_t t)
    {
        _pImpl = new TimerImpl(t);
    }

    timer::~timer()
    {
        delete _pImpl;
    }

    void timer::printCurrentTime(const char* format, std::ostream& os)
    {
        _pImpl->printCurrentTime(format,os);
    }

    timer::_Tp& timer::getStartTp()
    {
        return _pImpl->getStartTp();
        // TODO: 在此处插入 return 语句
    }

    timer::_Tp& timer::getCurrTp()
    {
        return _pImpl->getCurrTp();
    }

    const char* timer::to_time_str(const _Tp& timepoint)
    {
        return _pImpl->to_time_str(timepoint);
    }

    const char* timer::to_time_str(_Tp&& timepoint)
    {
        return _pImpl->to_time_str(timepoint);
    }

    const char* timer::to_time_str(time_t t)
    {
        return _pImpl->to_time_str(t);
    }

    void timer::parseTime(const _Tp& timepoint)
    {
        _pImpl->parseTime(timepoint);
    }

    void timer::parseTime(_Tp&& timepoint)
    {
        _pImpl->parseTime(timepoint);
    }

    void timer::parseTime(time_t t)
    {
        _pImpl->parseTime(t);
    }

    time_t timer::to_time_t(const _Tp& timepoint)
    {
        return _pImpl->to_time_t(timepoint);
    }

    time_t timer::to_time_t(_Tp&& timepoint)
    {
        return _pImpl->to_time_t(timepoint);
    }

    timer::_Tp timer::from_time_t(time_t time)
    {
        return _pImpl->from_time_t(time);
    }

    timer::_Tp timer::now()
    {
        return _pImpl->now();
    }

    int timer::second()
    {
        return _pImpl->second();
    }

    int timer::minute()
    {
        return _pImpl->minute();
    }

    int timer::hour()
    {
        return _pImpl->hour();
    }

    int timer::hourFormat12()
    {
        return _pImpl->hourFormat12();
    }

    int timer::weekday()
    {
        return _pImpl->weekday();
    }

    int timer::day()
    {
        return _pImpl->day();
    }

    int timer::month()
    {
        return _pImpl->month();
    }

    int timer::year()
    {
        return _pImpl->year();
    }

    bool timer::isAM()
    {
        return _pImpl->isAM();
    }

    bool timer::isPM()
    {
        return _pImpl->isPM();
    }

    //==========================
    template<>
    int timer::second<timer::_Tp>(timer::_Tp t) {
        return _pImpl->second(t);
    }
    template<>
    int timer::second<time_t>(time_t t) {
        return _pImpl->second(t);
    }

    template<>
    int timer::minute<timer::_Tp>(timer::_Tp t) {
        return _pImpl->minute(t);
    }
    template<>
    int timer::minute<time_t>(time_t t) {
        return _pImpl->minute(t);
    }

    template<>
    int timer::hour<timer::_Tp>(timer::_Tp t) {
        return _pImpl->hour(t);
    }

    template<>
    int timer::hour<time_t>(time_t t) {
        return _pImpl->hour(t);
    }

    template<>
    int timer::hourFormat12<timer::_Tp>(timer::_Tp t) {
        return _pImpl->hourFormat12(t);
    }

    template<>
    int timer::hourFormat12<time_t>(time_t t) {
        return _pImpl->hourFormat12(t);
    }

    template<>
    int timer::weekday<timer::_Tp>(timer::_Tp t) {
        return _pImpl->weekday(t);
    }

    template<>
    int timer::weekday<time_t>(time_t t) {
        return _pImpl->weekday(t);
    }

    template<>
    int timer::day<timer::_Tp>(timer::_Tp t) {
        return _pImpl->day(t);
    }

    template<>
    int timer::day<time_t>(time_t t) {
        return _pImpl->day(t);
    }

    template<>
    int timer::month<timer::_Tp>(timer::_Tp t) {
        return _pImpl->month(t);
    }

    template<>
    int timer::month<time_t>(time_t t) {
        return _pImpl->month(t);
    }

    template<>
    int timer::year<timer::_Tp>(timer::_Tp t) {
        return _pImpl->year(t);
    }

    template<>
    int timer::year<time_t>(time_t t) {
        return _pImpl->year(t);
    }

}