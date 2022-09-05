#ifndef __OAR_THREAD_H__
#define __OAR_THREAD_H__

#include "Semaphore.h"
#include "ThisThread.h"

#include <functional>
#include <memory>
#include <pthread.h>
#include <atomic>
#include <assert.h>
#include <stdio.h>
#include <type_traits>
namespace oar {

#define IS_SAME_TYPE(T,U) std::is_same<std::remove_reference_t<T>,U>::value

  
  struct ThreadInfo;

  // pthread_create调用此函数，内部调用ThreadInfo::run()
  void* startInThread(void* arg);    
  
  class Thread {
  
  public:
    typedef std::function<void()> threadFunc;

    Thread()
      :_pthreadId(0),
       _name(""),
       _joined(false),
       _started(false),
       _tid(0),
       _sem(0)
    {
      //      printf("default\n");
      setDefaultName();
    }
    /*
      explicit Thread(threadFunc f,const std::string& name = std::string())
      :_pthreadId(0),
      _name(""),
      _joined(false),
      _started(false),
      _tid(0)
      {
      printf("name\n");
      }
    */
    template<typename Func,typename ...Args>
    explicit Thread(Func &&f,Args && ...args);

    ~Thread();
    bool joinable() { return !_joined; }

    bool started() { return _started; }

    void start();
    int join();
    int detach();
    pid_t tid() { return _tid; }

    const char* nameStr() const { return _name.c_str(); }

    std::string name() const { return _name; }

    template<typename Func,typename ...Args>
    bool setFunc(Func &&f,Args && ...args);

    void setName(std::string name) { _name = name; }

    void setDefaultName();

    static int threadNum() { return __threadNums.load();}

  private:
    pthread_t _pthreadId;
    std::string _name;
    bool _joined;
    bool _started;
    pid_t _tid;
    threadFunc _func;
    Sem _sem;

  public:
    static std::atomic_int __threadNums;

  };

  
  // 放头文件才生效
  template<typename Func,typename ...Args>
  Thread::Thread(Func&& f,Args&& ...args)
    :_pthreadId(0),
     _name(""),
     _joined(false),
     _started(false),
     _tid(0),
     _func(std::bind(std::forward<Func>(f),std::forward<Args>(args)...)),
     _sem(0)
  {  
    setDefaultName();
  }

  
  template<typename Func,typename ...Args>
  bool Thread::setFunc(Func &&f,Args && ...args) {
    _func = std::bind(std::forward<Func>(f),std::forward<Args>(args)...);
  }





  
  struct ThreadInfo {
    using threadFunc = Thread::threadFunc;
    threadFunc _func;
    std::string _name;
    pid_t* _tid;
    Sem* _sem;
    
    ThreadInfo(threadFunc& func,std::string& name,pid_t* tid,Sem* sem)
      :_func(func),
       _name(name),
       _tid(tid),
       _sem(sem)
    {
    }

    void run() {
      *_tid = ThisThread::tid();
      _tid = nullptr;
      //      printf("before run() _sem->post()...\n");
      _sem->post();
      //      printf("after run() _sem->post()...\n");
      _sem = nullptr;

      try {
	//	printf("before run...\n");
	_func();
	//	printf("after run...\n");
      }
      catch(std::exception& ex) {
	fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
	fprintf(stderr, "reason: %s\n", ex.what());
	abort();
      }
      // post不能放这里，会一直阻塞 看例子../test/blockingqueueTest.cc
      //      printf("before run() _sem->post()...\n");
      //      _sem->post();
      //      printf("after run() _sem->post()...\n");
      //      _sem = nullptr;
    }
    
  };

  template<typename Func,typename ...Args>
  Thread::threadFunc convertFunc(Func &&f,Args && ...args) {
    //  return Func(std::bind(std::forward<Func>(f),std::forward<Args>(args)...));
    return std::bind(std::forward<Func>(f),std::forward<Args>(args)...);
  }
  

}


#endif
