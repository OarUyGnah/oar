#include "oar.h"
//#include "src/oartimer.cpp"
#define PAUSE system("pause")
using namespace std;



int main()
{
	oar::timer t;
	const char* str = t.to_time_str(t.getStartTp());
	cout << str << endl;
	t.printCurrentTime("123"/*, cout*/);

	t.printTimeStruct();
	/*cout << t.hour() << endl;
	cout << t.hour(t.getCurrTp()) << endl;
	cout << t.hourFormat12() << endl;
	cout << t.isAM() << endl;*/
	cout << t.to_time_t(t.now()) << endl;
	_sleep(2000);
	cout << t.to_time_t(t.now()) << endl;

	/*cout << t.second() << endl;
	cout << t.second(t.getCurrTp()) << endl;
	cout << t.minute() << endl;
	cout << t.minute(t.to_time_t(t.getCurrTp())) << endl;
	cout << t.hourFormat12() << endl;
	cout << t.hourFormat12(t.to_time_t(t.getCurrTp())) << endl;

	cout << t.weekday() << endl;
	cout << t.weekday(t.to_time_t(t.getCurrTp())) << endl;
	cout << "=======" << endl;

	cout << t.day() << endl;
	cout << t.day(t.to_time_t(t.getCurrTp())) << endl;
	cout << "=======" << endl;

	cout << t.month() << endl;
	cout << t.month(t.to_time_t(t.getCurrTp())) << endl;
	cout << "=======" << endl;
	cout << t.year() << endl;
	cout << t.year(t.to_time_t(t.getCurrTp())) << endl;*/
	cout << "=================" << endl;
	cout << t.to_time_str() << endl;
	std::chrono::system_clock::time_point tmp;
	auto&& test = tmp;
	cout << "tmp:" << t.to_time_str(test) << endl;
	cout << t.to_time_str(std::move(time_t(1657180800))) << endl;
	cout << t.to_time_str(SECS_YR_2000) << endl;
	
	PAUSE;
	return 0;
}
