#include "oar.h"
//#include "src/oartimer.cpp"
#include <vector>

#define PAUSE system("pause")
using namespace std;



int main()
{
	oar::timer t1;
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

	_sleep(3000);
	t1.parseTime();
	cout << "after 3 seconds " << t1.to_time_str() << '\n';

	auto timing1 = t1.now();
	_sleep(100);
	auto timing2 = t1.now();
	cout << "100ms " << (timing2 - timing1).count() << "\n";
	PAUSE;
	return 0;
}
