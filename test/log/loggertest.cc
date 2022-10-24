#include "../../oar/log.h"
#include <iostream>
using namespace std;
using namespace oar;

int main() {
  // LoggerManager
  Logger::LoggerPtr logger(new Logger);
  cout << logger->getName() << endl;
  LogEventWrapper wrapper(LogEvent::eventPtr(
      new LogEvent(logger, LogLevel::DEBUG, __FILENAME__, __LINE__, 0,
                   ThisThread::threadId, TimeStamp(), ThisThread::threadName)));
  wrapper.getStream() << "testing log";
  cout << "content ==> " << wrapper.getEvent()->content() << endl;
  // sleep(1000);
  return 0;
}