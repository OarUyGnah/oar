#include <pthread.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include <stdio.h>
 void *thread_function(void *arg) {
  int i;
  for ( i=0; i<8; i++) {
    printf("Thread working...! %d \n",i);
    sleep(1);
  }
  return NULL;
}
int main(void) {
  pthread_t mythread;
  
  if ( pthread_create( &mythread, NULL, thread_function, NULL) ) {
    printf("error creating thread.");
    abort();
  }
  printf("created\n");
  if ( pthread_join ( mythread, NULL ) ) {
    printf("error join thread.");
    abort();
  }
  printf("thread done! \n");
  exit(0);
}
