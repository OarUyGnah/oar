#include "oar.h"
//#include "src/oartimer.cpp"
#include <vector>

#define PAUSE system("pause")
using namespace std;

void testGetCurrFunc(oar::timer& t) {
	cout << t.getCurrSecond() << ' ' << t.getCurrMinute() << ' ' << t.getCurrHour() << ' ' << t.getCurrDay() << ' ' << t.getCurrMonth()
		<< t.getCurrYear() << endl;
}
void testGetStartFunc(oar::timer& t) {
	cout << t.getStartSecond() << ' ' << t.getStartMinute() << ' ' << t.getStartHour() << ' ' << t.getStartDay() << ' ' << t.getStartMonth()
		<< t.getStartYear() << endl;
}
int main()
{
	
	/*oar::timer t1;
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	oar::timer t2(tp);
	oar::timer t3(std::chrono::system_clock::now());
	oar::timer t4(time(nullptr));

	cout << "second " << t1.second() << '\n';
	cout << "minute " << t1.minute() << '\n';
	cout << "hour " << t1.hour() << '\n';
	cout << "hourformat12 " << t1.hourFormat12() << '\n';
	cout << "weekday " << t1.weekday() << '\n';
	cout << "day " << t1.day() << '\n';
	cout << "month " << t1.month() << '\n';
	cout << "year " << t1.year() << '\n';

	cout << "t1.to_time_t(t1.getCurrTp()) " << t1.to_time_t(t1.getCurrTp()) << '\n';
	cout << "t1.to_time_t(t1.getStartTp()) " << t1.to_time_t(t1.getStartTp()) << '\n';

	cout << "t1.to_time_str() " << t1.to_time_str() << '\n';
	cout << "t1.to_time_str(t1.getCurrTp()) " << t1.to_time_str(t1.getCurrTp()) << '\n';
	cout << "t1.to_time_str(t1.to_time_t(t1.getStartTp())) " << t1.to_time_str(t1.to_time_t(t1.getStartTp())) << '\n';
	
	cout << t1.to_time_str(t1.from_time_t(t1.to_time_t(t1.getStartTp()))) << '\n';

	oar::SleepMillis(3000);
	t1.parseTime();
	cout << "after 3 seconds " << t1.to_time_str() << '\n';

	auto timing1 = t1.now();
	oar::SleepMillis(1000);
	auto timing2 = t1.now();
	cout << "1000ms " << (timing2 - timing1).count() << "\n";*/

	cout << "======================" << endl;
	oar::timer testtimer((time_t)1646227800);//默认为SECOND  将初始化参数设置为start时间点
	//cout << testtimer.to_time_str(testtimer.getStartTp()) << endl;
	//cout << testtimer.to_time_str() << endl;//无参默认为构造时的数值
	//cout << testtimer.to_time_str(testtimer.now()) << endl;
	//cout << testtimer.to_time_str(1657355305) << endl;//second
	//cout << "after parsetime.." << endl;
	//testtimer.parseTime();//无参默认解析构造参数
	//cout << testtimer.to_time_str() << endl;
	//cout << "======================" << endl;
	//auto tp = std::chrono::system_clock::from_time_t((time_t)1657355305);
	//cout << "testtimer.to_time_t(tp)" << testtimer.to_time_t(tp) << endl;
	//cout << testtimer.to_time_t() << endl;
	//cout << testtimer.to_time_str(testtimer.getStartTp()) << endl;
	//cout << testtimer.to_time_str(testtimer.getCurrTp()) << endl;
	//cout << "init second " << testtimer.second() << endl;
	//oar::SleepMillis(3000);
	//cout << "parse after" << testtimer.second(testtimer.now()) << endl;

	//cout << "testtimer.second(testtimer.getStartTp()) " << testtimer.second(testtimer.getStartTp()) << endl;

	//cout << "..." << testtimer.second() << endl;
	//oar::SleepMillis(3000);
	//testtimer.parseTime();//parsetime每次运行会刷新_Ts_Dynamic结构体
 //                         //无参数的parseTime会将_Ts_Dynamic刷新为初始start的数值
 //                         //有参数则将_Ts_Dynamic数据刷新为当前参数的数值	
	//cout << "..." << testtimer.second() << endl; //无参数的second、minute等参数不会刷新_Ts_Dynamic      isAM isPM会刷新
	//cout << "..." << testtimer.second() << endl; //有参数则会刷新为新的_Ts_Dynamic数据
	//                                             //若之前使用过有参的parseTime或second等函数
 //                                                //因此使用时要用t.second(oar::timer::now())或t.second(t.getStartTp())获得准确时间	
 //                                                //建议使用getStart getCurr,,,以获得相应数据，不过仍会刷新结构体	_Ts_Dynamic
	//cout << "static now" << testtimer.hour(oar::timer::now()) << endl;
	//cout << "getstartSecond ..." << testtimer.getStartSecond() << endl;
	/*testGetCurrFunc(testtimer);
	cout << testtimer.second() << endl;
	cout << "=================" << endl;
	oar::SleepMillis(3000);
	testGetCurrFunc(testtimer);
	cout << "=================" << endl;
	testGetStartFunc(testtimer);
	cout << testtimer.second() << endl;*/

	cout << testtimer.to_time_str() << endl;
	cout << testtimer.getStartDay() << endl;
	cout << testtimer.day(oar::timer::now()) << endl;

	oar::SleepMillis(3000);
	oar::timer t2(oar::timer::now());
	cout << t2.to_time_str() << endl;

	oar::timer t3;
	t3 = time(nullptr);
	auto tp = oar::timer::now();
	t3 = tp;
	auto&& x = oar::timer::now();
	t3 = std::move(x);
	cout << t3.getCurrDay() << endl;
	//PAUSE;
	return 0;
}
