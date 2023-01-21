#include "oar/Thread.h"
#include "oar/Threadpool.h"
#include "oar/log.h"
#include <cstdio>
#include <iostream>
#include <iterator>
#include <memory>
#include <oar/ThisThread.h>
#include <stdexcept>
#include <string>
using namespace std;
using namespace oar;

void normalTest()
{
    // logfile
    // Logger::LoggerPtr l1(oar::std_logger);
    // oar::std_logger->addAppender(oar::std_appender);
    // l1->addAppender(oar::std_appender);
    // auto mgr = LoggerMgr.getInstance();
    // mgr->insert("first", l1);
    // LOG_DEBUG(l1) << "l1 test 2131242"
    //               << "????12321das";
    // LOG_INFO(l1) << "l1 info..."
    //              << "12312";
    // LOG_WARN(l1) << "l1 warning.....";

    // LOG_ERROR(l1) << "l1 error ....";

    // LOG_FATAL(l1) << "l1 fatal ....";

    // LOG_FORMAT_DEBUG(l1, "%d %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
    // LOG_FORMAT_INFO(l1, "%d %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
    // LOG_FORMAT_WARN(l1, "%d %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
    // LOG_FORMAT_ERROR(l1, "%d %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);
    // LOG_FORMAT_FATAL(l1, "[%d] %f %s %c %x", 100, 3.14, "testing %s", 'c', &l1);

    // stdout
    Debug << "test debug";
    Warn << "test warn";
    Info << "test info";
    Error << "test error";
    Fatal << "test fatal";
    FDebug("%d %f %s %c", 100, 3.14, "testing %s", 'c');
    FInfo("%d %f %s %c", 100, 3.14, "testing %s", 'c');
    FWarn("%d %f %s %c", 100, 3.14, "testing %s", 'c');
    FError("%d %f %s %c", 100, 3.14, "testing %s", 'c');
    FFatal("%d %f %s %c", 100, 3.14, "testing %s", 'c');
}

void threadTest()
{
    Threadpool pool(5, "testpool");
    pool.start();
    for (int i = 0; i < 10; ++i) {
        pool.runTask([=]() {
            for (int j = 0; j < 10; j++) {
                Debug << "test debug";
                Warn << "test warn";
                Info << "test info";
                Error << "test error";
                Fatal << "test fatal";
                FDebug("%d %f %s %c", 100, 3.14, "testing %s", 'c');
                FInfo("%d %f %s %c", 100, 3.14, "testing %s", 'c');
                FWarn("%d %f %s %c", 100, 3.14, "testing %s", 'c');
                FError("%d %f %s %c", 100, 3.14, "testing %s", 'c');
                FFatal("%d %f %s %c", 100, 3.14, "testing %s", 'c');
                // printf("tid : %d, %d\n", ThisThread::tid(), j);
                sleep(1);
            }
        });
    }
    printf("123\n");
    pool.stopRunNewTask();
}
int main()
{
    // normalTest();
    threadTest();
    return 0;
}