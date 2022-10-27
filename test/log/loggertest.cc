#include "../../oar/Thread.h"
#include "../../oar/Threadpool.h"
#include "../../oar/log.h"
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
using namespace std;
using namespace oar;

void normalTest() {
  // LoggerMgr m;
  Logger::LoggerPtr l1(new Logger("l1 logger"));
  Logger::LoggerPtr l2(new Logger("l2 logger"));
  l1->addAppender(std::make_shared<FileAppender>("first.log"));
  l2->addAppender(std::make_shared<FileAppender>("second.log"));
  auto mgr = LoggerMgr.getInstance();
  mgr->insert("first", l1);
  mgr->insert("second", l2);
  LOG_DEBUG(l1) << "l1 test 2131242"
                << "????12321das";
  LOG_DEBUG(l2) << "l2 test 2131242"
                << "????121212";
  LOG_INFO(l1) << "l1 info..."
               << "12312";
  LOG_INFO(l2) << "l2 info..."
               << ">>>>";
  LOG_WARN(l1) << "l1 warning.....";
  LOG_WARN(l2) << "l2 warning.....";
  LOG_ERROR(l1) << "l1 error ....";
  LOG_ERROR(l2) << "l2 error ....";
  LOG_FATAL(l1) << "l1 fatal ....";
  LOG_FATAL(l2) << "l2 fatal ....";

  LOG_FORMAT_DEBUG(l1, "%d %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
  LOG_FORMAT_INFO(l1, "%d %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
  LOG_FORMAT_WARN(l1, "%d %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
  LOG_FORMAT_ERROR(l1, "%d %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
  LOG_FORMAT_FATAL(l1, "[%d] %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
}

void threadTest() {
  // LoggerMgr m;
  Threadpool pool(5, "testpool");
  pool.start();
  for (int i = 0; i < 10; ++i) {
    pool.runTask([=]() {
      Logger::LoggerPtr l(
          new Logger(std::string(std::to_string(i) + "logger")));
      l->addAppender(std::make_shared<FileAppender>(
          std::string(std::to_string(i) + ".log")));
      for (int j = 0; j < 10; j++) {
        LOG_DEBUG(l) << "times : " << j;
        LOG_WARN(l) << "warn...";
        sleep(1);
      }
    });
  }
  pool.stopRunNewTask();
}
int main() {
  // normalTest();
  threadTest();
  return 0;
}