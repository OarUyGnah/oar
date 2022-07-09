#include "oartimer.h"

namespace oar {    


    timer::timer():_pImpl(new TimerImpl)
    {        
        parseTime();
    }

    timer::timer(const _Tp& tp) :_pImpl(new TimerImpl(tp))
    {        
        parseTime();
    }

    timer::timer(_Tp&& t): _pImpl(new TimerImpl(std::move(t)))
    {        
        parseTime();
    }

    

    timer::timer(time_t millseconds, TIMETYPE type)
    {
        switch (type)
        {
        case oar::SECOND:
            _pImpl = new TimerImpl(millseconds);
            break;
        case oar::MILLSECOND:
            _pImpl = new TimerImpl(millseconds / 1000);
            break;
        case oar::MICROSECOND:
            break;
        default:
            break;
        }
        parseTime();
    }

    timer::~timer()
    {
        delete _pImpl;
    }

    void timer::printCurrentTime(const char* format, std::ostream& os)
    {
        os << to_time_str(getCurrTp()) << '\n';
    }

    timer::_Tp& timer::getStartTp()
    {
        return _pImpl->start;
        // TODO: 在此处插入 return 语句
    }

    timer::_Tp& timer::getCurrTp()
    {
        return _pImpl->current;
    }

    

    //==========================
    template<>
    int timer::second<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return _pImpl->ts->second;
    }
    template<>
    int timer::second<time_t>(time_t t) {
        parseTime(t);
        return _pImpl->ts->second;
    }

    template<>
    int timer::minute<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return _pImpl->ts->minute;
    }
    template<>
    int timer::minute<time_t>(time_t t) {
        parseTime(t);
        return _pImpl->ts->minute;
    }

    template<>
    int timer::hour<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return _pImpl->ts->hour;
    }

    template<>
    int timer::hour<time_t>(time_t t) {
        parseTime(t);
        return _pImpl->ts->hour;
    }

    template<>
    int timer::hourFormat12<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        int res;
        return hoursFormatTo12(_pImpl->ts->hour, res);
    }

    template<>
    int timer::hourFormat12<time_t>(time_t t) {
        parseTime(t);
        int res;
        return hoursFormatTo12(_pImpl->ts->hour, res);
    }

    template<>
    int timer::weekday<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return _pImpl->ts->weekday;
    }

    template<>
    int timer::weekday<time_t>(time_t t) {
        parseTime(t);
        return _pImpl->ts->weekday;
    }

    template<>
    int timer::day<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return _pImpl->ts->day;
    }

    template<>
    int timer::day<time_t>(time_t t) {
        parseTime(t);
        return _pImpl->ts->day;
    }

    template<>
    int timer::month<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return _pImpl->ts->month;
    }

    template<>
    int timer::month<time_t>(time_t t) {
        parseTime(t);
        return _pImpl->ts->month;
    }

    template<>
    int timer::year<timer::_Tp>(timer::_Tp t) {
        parseTime(t);
        return _pImpl->ts->year;
    }

    template<>
    int timer::year<time_t>(time_t t) {
        parseTime(t);
        return _pImpl->ts->year;
    }

    
    

    time_t timer::to_time_t(const _Tp& timepoint)
    {
        return __To_Time_T(timepoint);
    }

    time_t timer::to_time_t(_Tp&& timepoint)
    {
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
        return _pImpl->ts->second;
    }

    int timer::minute()
    {
        return _pImpl->ts->minute;
    }

    int timer::hour()
    {
        return _pImpl->ts->hour;
    }

    int timer::hourFormat12()
    {
        parseTime();
        int res;
        return hoursFormatTo12(_pImpl->ts->hour, res); 
    }

    int timer::weekday()
    {
        return _pImpl->ts->weekday;
    }

    int timer::day()
    {
        return _pImpl->ts->day;
    }

    int timer::month()
    {
        return _pImpl->ts->month;
    }

    int timer::year()
    {
        return _pImpl->ts->year;
    }

    bool timer::isAM()
    {
        return !isPM();
    }

    bool timer::isPM()
    {
        return hour(_Clock::now()) >= 12;
    }

    


    /*******************   parseTime   *******************/
    void timer::__Parse_Time(time_t t)
    {
        //more compact version of the C library localtime function
        //year is offset from 1970
        int year;
        int month, monthLength;
        time_t time;
        unsigned long days;

        time = t;
        _pImpl->current = _Clock::now();
        _pImpl->ts->second = time % 60;
        time /= 60;
        _pImpl->ts->minute = time % 60;
        time /= 60;
        _pImpl->ts->hour = time % 24 + 8;//北京时间东八区+8
        time /= 24;
        _pImpl->ts->weekday = ((time + 4) % 7);
        year = 0;
        days = 0;
        while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
            year++;
        }
        _pImpl->ts->year = year + 1970;
        days -= LEAP_YEAR(year) ? 366 : 365;
        time -= days; // now it is days in this year, starting at 0

        days = 0;
        month = 0;
        monthLength = 0;
        for (month = 0; month < 12; month++) {
            if (month == 1) { // february
                if (LEAP_YEAR(_pImpl->ts->year)) {
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
        _pImpl->ts->month = month + 1;    
        _pImpl->ts->day = time + 1;     // day of month
    }

    template<>
    void timer::__Parse_Time_Aux<TimerImpl::_Tp>(TimerImpl::_Tp timepoint)
    {
        __Parse_Time(_Clock::to_time_t(timepoint));
    }

    template<>
    void timer::__Parse_Time_Aux<time_t>(time_t t) {
        __Parse_Time(t);
    }
    
    void timer::parseTime(const timer::_Tp& timepoint)
    {
        __Parse_Time_Aux(std::forward<const _Tp>(timepoint));
    }

    void timer::parseTime(_Tp&& timepoint)
    {
        __Parse_Time_Aux(std::forward<_Tp>(timepoint));
    }

    void timer::parseTime(time_t t) {
        __Parse_Time_Aux(t);
    }

    /*******************   to_time_str   *******************/

    const char* timer::__To_Time_Str(time_t tt) {
        *(_pImpl->timestr) = std::string(ctime(&tt));
        //const char* res = _pImpl->timestr->c_str();
        return _pImpl->timestr->c_str();
    }

    template<>
    const char* timer::__To_Time_Str_Aux<timer::_Tp>(timer::_Tp timepoint)//only work
    {
        //std::cout << "_Tp" << std::endl;
        //_Tp tmp = std::move(timepoint);
        return __To_Time_Str(_Clock::to_time_t(timepoint));
    }

    template<>
    const char* timer::__To_Time_Str_Aux<const timer::_Tp&>(const timer::_Tp& timepoint)
    {
        //std::cout << "_Tp&" << std::endl;
        return __To_Time_Str(_Clock::to_time_t(timepoint));
    }

    template<>
    const char* timer::__To_Time_Str_Aux<timer::_Tp&&>(timer::_Tp&& timepoint)
    {
        //std::cout << "_Tp&&" << std::endl;
        return __To_Time_Str(_Clock::to_time_t(timepoint));
    }

    template<>
    const char* timer::__To_Time_Str_Aux<time_t>(time_t t)
    {
        //std::cout << "time_t" << std::endl;
        return __To_Time_Str(t);
    }
    
    const char* timer::to_time_str(const _Tp& timepoint)
    {
        return __To_Time_Str_Aux(std::forward<const _Tp>(timepoint));
    }

    const char* timer::to_time_str(_Tp&& timepoint)
    {
        return __To_Time_Str_Aux(std::forward<_Tp>(timepoint));
    }

    const char* timer::to_time_str(time_t t)
    {
        return __To_Time_Str_Aux(t);
    }
    
    time_t timer::__To_Time_T(_Tp timepoint)
    {
        return _Clock::to_time_t(/*std::forward<_Tp&>*/(timepoint));//rvalue waiting...
    }

   //===========================================================
	void TimerImpl::init() {
        timestr = new std::string;
        ts = new time_struct;
        current = start;
    }

    TimerImpl::TimerImpl()
    {
        start = _Clock::now();
        init();
    }

    TimerImpl::TimerImpl(const _Tp& tp)
    {
        start = tp;
        init();
    }

    TimerImpl::TimerImpl(_Tp&& tp)
    {
        start = std::move(tp);
        init();
    }

    TimerImpl::TimerImpl(time_t t)
    {
        start = _Clock::from_time_t(t);
        init();
    }

    TimerImpl::~TimerImpl()
    {
        std::cout << "dtor impl..." << std::endl;
        delete ts;
        delete timestr;
    }

}