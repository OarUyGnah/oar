#include "../oar/Thread.h"
#include "../oar/ThisThread.h"
#include <stdio.h>

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
  
  Thread t(test);
  t.start();
  t.join();

  Thread t1([]() {printf("lambda %d\n",100);});
  t1.start();

  printf("=========\n");
  Thread t2(std::bind(test2,2),"213");
  t2.start();
  printf("=========\n");
  return 0;
}
