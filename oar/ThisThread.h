#ifndef __THISTHREAD_H__
#define __THISTHREAD_H__

#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
namespace oar {

  namespace ThisThread {
    extern __thread int  threadId;
    extern __thread char threadIdString[32];
    extern __thread int  threadIdstringLen;
    extern __thread const char* threadName;

    inline void cacheTid() {
      // new thread
      if(threadId == 0) {
	threadId = static_cast<int>(::syscall(SYS_gettid));
	threadIdstringLen = snprintf(threadIdString,sizeof(threadIdString),"%5d",threadId);
      }
    }

    inline int tid() {
      if(__builtin_expect(threadId == 0,0))
	cacheTid();
      return threadId;
    }

    inline const char* tidString() {
      if(__builtin_expect(threadId == 0,0))
	cacheTid();
      return threadIdString;
    }

    inline int tidStringLen() {
      if(__builtin_expect(threadId == 0,0))
	cacheTid();
      return threadIdstringLen;
    }

    inline const char* name() {
      if(__builtin_expect(threadId == 0,0))
	cacheTid();
      return threadName;
    }

    //bool isMainThread();


  }

  
}



#endif
