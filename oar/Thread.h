#ifndef __OAR_THREAD_H__
#define __OAR_THREAD_H__
#include <functional>
#include <memory>
#include <pthread.h>
#include <atomic>
#include <assert.h>

namespace oar {

  struct ThreadInfo;

  // pthread_create调用此函数，内部调用ThreadInfo::run()
  void* startInThread(void* arg);    
  
  class Thread {
  
  public:
    typedef std::function<void()> threadFunc;
    Thread(threadFunc func,const std::string& name = std::string());
    
    
    //    template<typename Func,typename ...Args>
    //Thread(Func &&f,Args && ...args);
    
    
    ~Thread();
    bool joinable() { return !_joined; }
    bool started() { return _started; }
    void start();
    int join();
    pid_t tid() { return _tid; }

    //    static int threadNum() { return __threadNum. ;}
    //static atomic<int> __threadNum; 
  private:

    pthread_t _pthreadId;
    threadFunc _func;
    std::string _name;
    pid_t _tid;
    bool _joined;
    bool _started;

  };


  struct ThreadInfo {
    using threadFunc = Thread::threadFunc;
    threadFunc _func;
    std::string _name;
    pid_t* _tid;
    
    ThreadInfo(threadFunc func,std::string& name,pid_t* tid)
      :_func(func),
       _name(name),
       _tid(tid)
    {
    }

    void run() {
      try {
	_func();
      }
      catch(std::exception& ex) {
	fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
	fprintf(stderr, "reason: %s\n", ex.what());
	abort();
      }
    }
    
  };  


}


#endif
