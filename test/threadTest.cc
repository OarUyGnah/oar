#include "../oar/Thread.h"
#include "../oar/ThisThread.h"
#include <stdio.h>

/*
  g++ threadTest.cc ../oar/Thread.cc ../oar/ThisThread.cc ../oar/Semaphore.cc -lpthread
*/


using namespace oar;
void test() {
  for (int i = 0;i< 100;++i)
    printf("current i is %d\n",i);

}
void test2(int x)
{
  printf("tid=%d, x=%d\n", ThisThread::tid(), x);
}
int main() {
  /*
    Thread 只接受返回值void的函数体
  */

  std::function<void()> stdfunc(test);
  Thread t(stdfunc);
  t.start();
  t.join();
  printf("t.name is %s\n",t.nameStr());
  Thread t1([]() {printf("lambda %d\n",100);});
  t1.start();
  t1.join();
  printf("=========\n");
  printf("t1.name is %s\n",t1.nameStr());
  Thread t2(std::bind(test2,2),"213");
  t2.start();
  t2.join();
  printf("=========\n");
  printf("t2.name is %s\n",t2.nameStr());
  Thread t3([](){test2(10000);});
  t3.start();
  t3.join();
  printf("=========\n");
  printf("t3.name is %s\n",t3.nameStr());
  {
    Thread t4;
    t4.setFunc([](int i){test2(i);},20000);
    t4.setName("t4...");
    t4.start();
    t4.join();
    printf("=========\n");
    printf("t4.name().c_str() is %s\n",t4.name().c_str());
  
    printf("total threadNums is %d\n",Thread::threadNum()); // total threadNums is 5
  }
  printf("total threadNums is %d\n",Thread::threadNum()); // total threadNums is 4

  Thread t5([](int num) {
    for(int i = 0; i < num; ++i) {
      printf("%s times %d\n","t5",i);
      sleep(1);
    }
  },10);
  t5.start();
  t5.join();

  Thread t6([](std::string&){printf("thread t6");},std::string("namet6"));
  t6.start();
  t6.join();
  
  return 0;
}
