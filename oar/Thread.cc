#include "Thread.h"
#include <string.h>
#include <stdio.h>
namespace oar {

  void* startInThread(void* arg) {
    ThreadInfo* info = static_cast<ThreadInfo*>(arg);
    //    printf("before startInthread info->run()\n");
    info->run();
    //    printf("after startInthread info->run()\n");
    return nullptr;
  }

  std::atomic_int Thread::__threadNums;

  Thread::~Thread() {
    if (_started && !_joined)
      pthread_detach(_pthreadId);
    Thread::__threadNums.fetch_sub(1, std::memory_order_relaxed);
  }

  void Thread::start() {
    assert(!_started);
    _started = true;
    ThreadInfo* info = new ThreadInfo(_func,_name,&_tid);
    pthread_create(&_pthreadId,nullptr,&startInThread,info);
  }

  int Thread::join() {
    assert(_started);
    assert(!_joined);
    _joined = true;
    return pthread_join(_pthreadId,nullptr);
  }

  /*
    template<typename Func,typename ...Args>
    auto convertFunc(Func &&f,Args && ...args) -> decltype(std::bind(std::forward<Func>(f),std::forward<Args>(args)...)){
    //  return Func(std::bind(std::forward<Func>(f),std::forward<Args>(args)...));
    return std::bind(std::forward<Func>(f),std::forward<Args>(args)...);
    }
  */

  void Thread::setDefaultName() {
    char buf[10];
    snprintf(buf,10,"Thread_%02d",threadNum() + 1);
    Thread::__threadNums.fetch_add(1, std::memory_order_relaxed);
    _name.append(buf,strlen(buf));
  }
}
