
#include <unistd.h>
//#include <stdio.h>
#include "../oar/Mutex.h"
//#include "./testHeader.h"

using namespace oar;


Mutex m;

void* func(void* arg) {
  MutexGuard mg(m);
  //  printf("begin lock\n");
  for (int i = 0 ;i < 10 ;++i) {
    usleep(100*1000);
    //    sleep(1);
    printf("time is %d, current pid is %d\n",i,gettid());
  }
  //  printf("1231\n");
  return nullptr;
}

int main() {
  // 必须初始化，否则段错误
  pthread_t t1 = 1;
  pthread_t t2 = 0;
  printf("t1 %d t2 %d\n",t1,t2);
  int arg1 = 1,arg2 = 2;
  pthread_create(&t1,nullptr,func,nullptr);
  printf("create 1\n");

  pthread_create(&t1,nullptr,func,(void*)&arg2);
  /*  if (pthread_tryjoin_np(t1, nullptr)) {
      printf("join t1 error\n");
      }else
      printf("t1 join success\n");
      if (pthread_tryjoin_np(t1, nullptr)) {
      printf("join t2 error\n");
      }else
      printf("t2 join success\n");*/
  pthread_join(t1, nullptr);
  pthread_join(t2, nullptr);
  
  
  //sleep(10);
  //sleep(3);
  for (int i = 0;i< 50 ;++i) {
    usleep(100*1000);
    printf("current pid is %d\n",gettid());
  }
  getchar();
  return 0;
}
