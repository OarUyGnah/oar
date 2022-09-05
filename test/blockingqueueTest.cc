#include "../oar/BlockingQueue.h"
#include "../oar/Thread.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <list>
using namespace oar;

BlockingQueue<int,std::list<int>> q;
bool isStop = false;

auto EXIT = [](int sig) {
  printf("signum is %d\n",sig);
  isStop = true;
  //exit(0);
};

void produce() {
  int times = 1;
  printf("produce\n");
  while(!isStop) {
    printf("push times %d\n",times);
    q.push(times++);
    sleep(1);
  }
}

void consume() {
  printf("consume\n");
  while(!isStop) {
    printf("current take is %d\n",q.take());
    sleep(1);
  }
}

int main() {
  signal(SIGINT,EXIT);
  signal(SIGTERM,EXIT);

  Thread producer(produce);
  Thread consumer(consume);
  producer.start();
  /* producer要用detach，否则主线程会等待，producer会一直push
     然而consumer会在sleep之后才start，所以无法take，导致一致阻塞
  */
  //producer.join();
  producer.detach();
  printf("before sleep(2)\n");
  sleep(2);
  printf("sleep(2) finish\n");
 
  consumer.start();
  consumer.join();

 
  return 0;
}
