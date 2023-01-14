#include "oar/Threadpool.h"
#include <stdio.h>

/*
  g++ threadpoolTest.cc ../oar/Thread.cc ../oar/Threadpool.cc ../oar/Semaphore.cc ../oar/ThisThread.cc  -lpthread
*/

using namespace oar;
using namespace std;

void task(int tasknum)
{
    for (int i = 0; i < 3; ++i) {
        printf("taskNum is %d\tcurrent looptime is %d\n", tasknum, i);
        sleep(1);
    }
}
auto LAMBDA = [](int num) { task(num); };

int main()
{

    Threadpool pool(5, "oar threadpool");
    pool.start();
    for (int i = 0; i < 20; ++i) {
        pool.runTask(std::bind(LAMBDA, i));
        printf("===============runTask %d times================\n", i);
    }
    printf("before stop.............\n");
    // 调用stop则后几个task不会运行，直接退出
    //  pool.stop();
    // 调用stopRunNewTask则会将当前已经在任务队列的任务做完再退出
    pool.stopRunNewTask();
    //  pool.runTask(std::bind(LAMBDA,1000));
    //  getchar();
    printf("%s end\n", pool.name().c_str());
    return 0;
}
