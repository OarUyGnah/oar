#include "../oar/CountDownLatch.h"
#include "../oar/Thread.h"
#include <iostream>
#include <unistd.h>

using namespace oar;
using namespace std;

CountDownLatch latch(4);

void test() {
  cout << "test start" << endl;
  //  latch.countDown();
  latch.wait();
  cout << "test end" << endl;
}

int main() {
  Thread t1(test);
  Thread t2(test);
  Thread t3(test);
  Thread t4(test);

  t1.start();
  t1.detach();
  t2.start();
  t2.detach();
  t3.start();
  t3.detach();
  t4.start();
  t4.detach();
  
  //  sleep(10);

  for(int i = 0;i < 4;++i) {
    getchar();
    printf("getchar %d\n",i);
    latch.countDown();
    printf("latch count = %d\n",latch.getCount());
  }
  return 0;
}
