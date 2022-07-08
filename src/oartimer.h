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
		timer(time_t t);
		timer(const timer&) = delete;
		~timer();
		void			printCurrentTime(const char* format, std::ostream& os = std::cout);
		_Tp&			getStartTp();
		_Tp&			getCurrTp();
		const char*		to_time_str(const _Tp& timepoint);
		const char*		to_time_str(_Tp&& timepoint = _Clock::now());
		const char*		to_time_str(time_t t);
		void			parseTime(const _Tp& timepoint);
		void			parseTime(_Tp && timepoint = _Clock::now());
		void			parseTime(time_t t);

		time_t			to_time_t(const _Tp& timepoint);
		time_t			to_time_t(_Tp&& timepoint = _Clock::now());
		_Tp				from_time_t(time_t time);
		_Tp				now();
		/*  time and date functions   */
		int				second();          // the second now 
		int				minute();          // the minute now
		int				hour();            // the hour now 
		int				hourFormat12();    // the hour now in 12 hour format
		int				weekday();         // the weekday now (Sunday is day 1) 
		int				day();             // the day now 
		int				month();           // the month now  (Jan is month 1)
		int				year();            // the full four digit year: (2009, 2010 etc) 
		bool			isAM();            // returns true if time now is AM
		bool			isPM();            // returns true if time now is PM


		template<typename T>
		int     second(T t);  // the second for the given time

		template<typename T>
		int     minute(T t);  // the minute for the given time

		template<typename T>
		int		hour(T t);

		template<typename T>
		int     hourFormat12(T t); // the hour for the given time in 12 hour format

		template<typename T>
		int     weekday(T t); // the weekday for the given time 

		template<typename T>
		int     day(T t);     // the day for the given time

		template<typename T>
		int     month(T t);   // the month for the given time

		template<typename T>
		int     year(T t);    // the year for the given time

		template<typename T>
		uint8_t isAM(time_t t);    // returns true the given time is AM	

		template<typename T>
		uint8_t isPM(T t);    // returns true the given time is PM
	};









	class TimerImpl {
	private:
		friend timer;
		using _Clock = std::chrono::system_clock;
		using _Tp = std::chrono::system_clock::time_point;
		using _Dur = std::chrono::system_clock::duration;

		_Tp current;
		_Tp start;
		_Tp end;
		//_Dur duration;
		std::string* timestr;
		time_struct *ts;
		//time_t tt;

		TimerImpl();
		void init();
	public:
		TimerImpl(const TimerImpl&) = delete;
		TimerImpl(time_t t);
		TimerImpl(const _Tp& t);
		TimerImpl(_Tp&& t);

		~TimerImpl();
		void			printCurrentTime(const char* format, std::ostream& os = std::cout);
		_Tp&			getStartTp();
		_Tp&			getCurrTp();

		const char*		to_time_str(const _Tp& timepoint);
		const char*		to_time_str(_Tp&& timepoint = _Clock::now());
		const char*		to_time_str(time_t t);
		
		void			parseTime(const _Tp& timepoint);
		void			parseTime(_Tp && = _Clock::now());
		void			parseTime(time_t t);

		time_t			to_time_t(const _Tp& timepoint);
		time_t			to_time_t(_Tp&& timepoint = _Clock::now());
		_Tp				from_time_t(time_t time);
		_Tp				now();



		


		/*  time and date functions   */
		int				second();          // the second now 
		int				minute();          // the minute now
		int				hour();            // the hour now 
		int				hourFormat12();    // the hour now in 12 hour format
		int				weekday();         // the weekday now (Sunday is day 1) 
		int				day();             // the day now 
		int				month();           // the month now  (Jan is month 1)
		int				year();            // the full four digit year: (2009, 2010 etc) 
		bool			isAM();            // returns true if time now is AM
		bool			isPM();            // returns true if time now is PM

		template<typename T>
		int     second(T t);  // the second for the given time

		template<typename T>
		int     minute(T t);  // the minute for the given time

		template<typename T>
		int		hour(T t);

		template<typename T>
		int     hourFormat12(T t); // the hour for the given time in 12 hour format

		template<typename T>
		int     weekday(T t); // the weekday for the given time 

		template<typename T>
		int     day(T t);     // the day for the given time

		template<typename T>
		int     month(T t);   // the month for the given time

		template<typename T>
		int     year(T t);    // the year for the given time

		template<typename T>
		uint8_t isAM(time_t t);    // returns true the given time is AM	

		template<typename T>
		uint8_t isPM(T t);    // returns true the given time is PM


	private:
		/**
			还需要在分析左右值的传递
		*/
		//void			__Parse_Time(_Tp timepoint);
		template<class T>
		void			__Parse_Time_Aux(T timepoint);
		void			__Parse_Time(time_t t);

		//const char*		__To_Time_Str(_Tp timepoint);
		template<class T>
		const char*		__To_Time_Str_Aux(T timepoint);
		const char*		__To_Time_Str(time_t t);

		time_t			__To_Time_T(_Tp timepoint);

		
	};

	
}


#endif // __OARTIMER_H__