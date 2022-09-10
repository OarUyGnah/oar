#include "../oar/Time.h"
#include <iostream>

using namespace oar;
using namespace std;

int main() {

  Date d1;
  cout << d1.toString() << endl;
  Date d2(time(nullptr));
  cout << d2.toString() << endl;
  Date d3(std::chrono::system_clock::now());
  cout << d3.toString() << endl;

  TimeZone tz1;
  cout << tz1.getZone() << endl;
  TimeZone tz2(-2);
  cout << tz2.getZone() << endl;

  //  cout << "to_time_t " << chrono::system_clock::to_time_t(chrono::system_clock::now()) << endl;

  TimeStamp ts;
  cout << "sizeof Timestamp is " << sizeof(ts) << endl;
  cout << ts.getmicroEpoch() << endl;
  Date d4 = ts.toDate();
  cout << d4.toString() << endl;
  cout << "ts.totime_t  " <<ts.totime_t() << endl;

  Date d5(ts);
  cout << d5.toString() << endl;
  cout << d5.getYear() << endl;

  Time time;
  TimeStamp ts2 = time.now();
  cout << ts2.totime_t() << endl;
  cout << "sinceEpoch SECOND  " << time.sinceEpoch() << endl;
  cout << "sinceEpoch MILLSECOND  " << time.sinceEpoch(Time::MILLSECOND) << endl;
  cout << "sinceEpoch MICROSECOND  " << time.sinceEpoch(Time::MICROSECOND) << endl;
  
  return 0;
}
