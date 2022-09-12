#include "../oar/Threadpool.h"

#include <stdio.h>

using namespace oar;
using namespace std;

void task(int tasknum) {
  for (int i = 0; i < 3; ++i) {
    printf("taskNum is %d\tcurrent looptime is %d\n", tasknum,i);
    sleep(1);
  }
}
auto LAMBDA =[](int num){task(num);};

int main() {

  Threadpool pool(5,"oar threadpool");
  pool.start();
  for (int i = 0; i < 20; ++i) {
    pool.runTask(std::bind(LAMBDA,i));
    printf("===============runTask %d times================\n",i);
  }
  printf("before stop.............\n");
  pool.stop();
  //  getchar();
  printf("%s end\n",pool.name().c_str());
  return 0;
}
