#include "Thread.h"
#include <stdio.h>
namespace oar {

  void* startInThread(void* arg) {
    ThreadInfo* info = static_cast<ThreadInfo*>(arg);
    printf("before startInthread info->run()\n");
    info->run();
    printf("after startInthread info->run()\n");
    return nullptr;
  }

  
  Thread::Thread(threadFunc func,const std::string& name)
    :_pthreadId(0),
     _name(name),
     _joined(false),
     _started(false),
     _tid(0),
     _func(func)
  {

  }
  /*
    template<typename Func,typename ...Args>
    Thread::Thread(Func&& f,Args&& ...args)
    :_pthreadId(0),
    _name(""),
    _joined(false),
    _started(false),
    _tid(0)
    {
    //printf("123\n");
    _func = std::bind(std::forward<Func>(f),std::forward<Args>(args)...);
    //  (threadFunc)convertFunc(std::forward<Func>(f),std::forward<Args>(args)...)();
    }
  */
  Thread::~Thread() {
    if (_started && !_joined)
      pthread_detach(_pthreadId);
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

  template<typename Func,typename ...Args>
  auto convertFunc(Func &&f,Args && ...args) -> decltype(std::bind(std::forward<Func>(f),std::forward<Args>(args)...)){
    //  return Func(std::bind(std::forward<Func>(f),std::forward<Args>(args)...));
    return std::bind(std::forward<Func>(f),std::forward<Args>(args)...);
  }

}
