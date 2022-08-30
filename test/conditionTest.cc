#include "../oar/Condition.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <list>

using namespace oar;

bool isStop = false;
Mutex m;
Condition cond(m);
std::list<int> list;
int times = 1;
  
auto EXIT = [](int sig) {
  printf("sig is %d\n", sig);
  isStop = true;
};

void* producer(void* arg) {
  while (!isStop) {
    printf("produce num is %d\n",times);
    list.push_back(times++);
    cond.notify();
    sleep(1);
  }
  cond.broadcast();
  printf("producer end...\n");
}

void* consumer(void* arg) {
  while (!isStop) {
    while (list.empty() && !isStop) {
      printf("inner while...\n");
      cond.wait();
    }
    if(!list.empty()){
      int x = list.front();
      list.pop_front();
      printf("consume num is %d\n", x);
    }
  }
  printf("consumer end...\n");
}

int main() {
  signal(SIGINT,EXIT);
  signal(SIGTERM, EXIT);
  pthread_t t1 = 0,t2 = 0;
  pthread_create(&t1, nullptr, producer, nullptr);
  pthread_create(&t2, nullptr, consumer, nullptr);
  pthread_join(t1, nullptr);
  pthread_join(t2, nullptr);

  printf("ending...\n");
  return 0;
}
