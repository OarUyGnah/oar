#include "../oar/Thread.h"
#include <stdio.h>

using namespace oar;
void test() {
  for (int i = 0;i< 100;++i)
    printf("current i is %d\n",i);

}
int main() {
  
  Thread t(test);
  t.start();
  t.join();

  Thread t1([]() {printf("lambda %d\n",100);});
  t1.start();
  return 0;
}