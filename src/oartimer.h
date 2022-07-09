#ifndef	__OARTIMER_H__
#define	__OARTIMER_H__



#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
namespace oar {
		
	   	/*========================================================================*/
#define LEAP_YEAR(Y)     ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )


#define  tmYearToCalendar(Y) ((Y) + 1970)  // full four digit year 
#define  CalendarYrToTm(Y)   ((Y) - 1970)
#define  tmYearToY2k(Y)      ((Y) - 30)    // offset is from 2000
#define  y2kYearToTm(Y)      ((Y) + 30)   

#define SECS_PER_MIN  ((time_t)(60UL))
#define SECS_PER_HOUR ((time_t)(3600UL))
#define SECS_PER_DAY  ((time_t)(SECS_PER_HOUR * 24UL))
#define DAYS_PER_WEEK ((time_t)(7UL))
#define SECS_PER_WEEK ((time_t)(SECS_PER_DAY * DAYS_PER_WEEK))
#define SECS_PER_YEAR ((time_t)(SECS_PER_DAY * 365UL)) // TODO: ought to handle leap years
#define SECS_YR_2000  ((time_t)(946684800UL)) // the time at the start of y2k


	/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) ((_time_) % SECS_PER_MIN)  
#define numberOfMinutes(_time_) (((_time_) / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (((_time_) % SECS_PER_DAY) / SECS_PER_HOUR)
#define dayOfWeek(_time_) ((((_time_) / SECS_PER_DAY + 4)  % DAYS_PER_WEEK)+1) // 1 = Sunday
#define elapsedDays(_time_) ((_time_) / SECS_PER_DAY)  // this is number of days since Jan 1 1970
#define elapsedSecsToday(_time_) ((_time_) % SECS_PER_DAY)   // the number of seconds since last midnight 
// The following macros are used in calculating alarms and assume the clock is set to a date later than Jan 1 1971
// Always set the correct time before setting alarms
#define previousMidnight(_time_) (((_time_) / SECS_PER_DAY) * SECS_PER_DAY)  // time at the start of the given day
#define nextMidnight(_time_) (previousMidnight(_time_)  + SECS_PER_DAY)   // time at the end of the given day 
#define elapsedSecsThisWeek(_time_) (elapsedSecsToday(_time_) +  ((dayOfWeek(_time_)-1) * SECS_PER_DAY))   // note that week starts on day 1
#define previousSunday(_time_) ((_time_) - elapsedSecsThisWeek(_time_))      // time at the start of the week for the given time
#define nextSunday(_time_) (previousSunday(_time_)+SECS_PER_WEEK)          // time at the end of the week for the given time

	/* Useful Macros for converting elapsed time to a time_t */
#define minutesToTime_t ((M)) ( (M) * SECS_PER_MIN)  
#define hoursToTime_t   ((H)) ( (H) * SECS_PER_HOUR)  
#define daysToTime_t    ((D)) ( (D) * SECS_PER_DAY) // fixed on Jul 22 2011
#define weeksToTime_t   ((W)) ( (W) * SECS_PER_WEEK)   
#define hoursFormatTo12(H,RES)  ((H == 0) ? (RES = 0) : (RES = (H > 12 ? H - 12 : H)))

		/*========================================================================*/
	enum TIMETYPE{
		SECOND,
		MILLSECOND,
		MICROSECOND
	};
	const uint8_t monthDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
class TimerImpl;
	struct time_struct {
		int second;
		int minute;
		int hour;
		int weekday;   // day of week, sunday is day 1
		int day;
		int month;
		int year;   // offset from 1970; 
	};

	class timer {
	private:
		using _Clock = std::chrono::system_clock;
		using _Tp = std::chrono::system_clock::time_point;
		using _Dur = std::chrono::system_clock::duration;

		TimerImpl* _pImpl;
	public:
		timer();
		timer(const _Tp& t);
		timer(_Tp&& t);
		timer(time_t seconds,TIMETYPE type = TIMETYPE::SECOND);//注意是s还是ms
		timer(const timer&) = delete;
		timer& operator=(const _Tp& tp);
		timer& operator=(_Tp&& tp);
		timer& operator=(time_t t);
		~timer();
		//静态函数，返回当前时间点
		static _Tp				now();

		void			printCurrentTime(const char* format, std::ostream& os = std::cout);
		//获取start时间点
		_Tp&			getStartTp();
		//获取current时间点
		_Tp&			getCurrTp();
		//根据时间点或time_t生成对应时间的字符串，time_t默认单位为秒，无参默认打印start时间点
		//不会影响_pImpl->_Ts_Dynamic结构体
		const char*		to_time_str(const _Tp& timepoint);
		const char*		to_time_str(_Tp&& timepoint);
		const char*     to_time_str(time_t t);
		const char*     to_time_str();
		
		//解析时间点或time_t，并更新_pImpl->_Ts_Dynamic结构体
		//无参默认解析start时间点
		//有参数将则将当前参数对应信息更新到_pImpl->_Ts_Dynamic结构体
		void			parseTime(const _Tp& timepoint);
		void			parseTime(_Tp && timepoint);
		void			parseTime(time_t t);
		void			parseTime();

		//时间点转换为time_t，无参默认转换start
		//不会影响_pImpl->_Ts_Dynamic结构体
		time_t			to_time_t(const _Tp& timepoint);
		time_t			to_time_t(_Tp&& timepoint);
		time_t			to_time_t();
		//通过给定time_t类型的time获取对应时间点
		_Tp				from_time_t(time_t time);

		
		//无参时间函数，打印当前_pImpl->_Ts_Dynamic结构体的对应数据
		//不推荐使用，因为会触发parseTime函数，导致_pImpl->_Ts_Dynamic结构体的内容改变
		int				second();          
		int				minute();          
		int				hour();            
		int				hourFormat12();    
		int				weekday();         
		int				day();             
		int				month();           
		int				year();            
		bool			isAM();            
		bool			isPM();            
		//返回start时间点的对应数据
		int getStartSecond();
		int getStartMinute();
		int getStartHour();
		int getStartHourFormat12();
		int getStartWeekday();
		int getStartDay();
		int getStartMonth();
		int getStartYear();
		bool startIsAM();
		bool startIsPM();
		//返回current时间点的对应数据
		int getCurrSecond();
		int getCurrMinute();
		int getCurrHour();
		int getCurrHourFormat12();
		int getCurrWeekday();
		int getCurrDay();
		int getCurrMonth();
		int getCurrYear();
		bool currIsAM();
		bool currIsPM();

		//有参时间函数模板，特化了timer::_Tp和time_t类型，根据参数获得对应的信息
		//内部调用parseTime(t) 导致_pImpl->_Ts_Dynamic结构体更新
		template<typename T>
		int     second(T t);  

		template<typename T>
		int     minute(T t);  

		template<typename T>
		int		hour(T t);

		template<typename T>
		int     hourFormat12(T t); 

		template<typename T>
		int     weekday(T t); 

		template<typename T>
		int     day(T t);     

		template<typename T>
		int     month(T t);   

		template<typename T>
		int     year(T t);    

		template<typename T>
		uint8_t isAM(time_t t);    

		template<typename T>
		uint8_t isPM(T t);    

	private:
		/**
			还需要在分析左右值的传递
		*/
		
		template<class T>
		void			__Parse_Time_Aux(T timepoint);
		void			__Parse_Time(time_t t);

		
		template<class T>
		const char* __To_Time_Str_Aux(T timepoint);
		const char* __To_Time_Str(time_t t);

		time_t			__To_Time_T(_Tp timepoint);
	};









	class TimerImpl {
	public:
		friend timer;
		using _Clock = std::chrono::system_clock;
		using _Tp = std::chrono::system_clock::time_point;
		using _Dur = std::chrono::system_clock::duration;

		_Tp current;
		_Tp start;
		_Tp end;
		//_Dur duration;
		std::string* timestr;
		time_struct* _Ts_init;
		time_struct* _Ts_Dynamic;
		//time_t tt;			   

	public:
		TimerImpl();
		TimerImpl(const TimerImpl&) = delete;
		TimerImpl(const _Tp&);
		TimerImpl(_Tp&&);
		TimerImpl(time_t);
		~TimerImpl();
		void init();



	};

	
}


#endif // __OARTIMER_H__