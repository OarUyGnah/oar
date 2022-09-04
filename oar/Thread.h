#ifndef __OAR_THREAD_H__
#define __OAR_THREAD_H__
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

    static std::atomic_int __threadNums;
    
    Thread()
      :_pthreadId(0),
       _name(""),
       _joined(false),
       _started(false),
       _tid(0)
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
    pid_t tid() { return _tid; }

    const char* nameStr() const { return _name.c_str(); }
    std::string name() const { return _name; }
    template<typename Func,typename ...Args>
    bool setFunc(Func &&f,Args && ...args);

    void setName(std::string name) {
      _name = name;
    }
    void setDefaultName();
    static int threadNum() { return __threadNums.load();}
    //static atomic<int> __threadNum; 
  private:

    pthread_t _pthreadId;
    threadFunc _func;
    std::string _name;
    pid_t _tid;
    bool _joined;
    bool _started;
    

  };

  
  // 放头文件才生效
  template<typename Func,typename ...Args>
  Thread::Thread(Func&& f,Args&& ...args)
    :_pthreadId(0),
     _name(""),
     _joined(false),
     _started(false),
     _tid(0),
     _func(std::bind(std::forward<Func>(f),std::forward<Args>(args)...))
  {  
    //    printf("F&&\n");
    setDefaultName();
  }

  
  /*
    template<>
    Thread::Thread<Thread::threadFunc,std::string>(threadFunc func,const std::string& name)
    :_pthreadId(0),
    _name(name),
    _joined(false),
    _started(false),
    _tid(0),
    _func(func)
    {
    printf("threadFunc\n");
    }
  */

  template<typename Func,typename ...Args>
  bool Thread::setFunc(Func &&f,Args && ...args) {
    _func = std::bind(std::forward<Func>(f),std::forward<Args>(args)...);
  }





  
  struct ThreadInfo {
    using threadFunc = Thread::threadFunc;
    threadFunc _func;
    std::string _name;
    pid_t* _tid;
    
    ThreadInfo(threadFunc& func,std::string& name,pid_t* tid)
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

  template<typename Func,typename ...Args>
  Thread::threadFunc convertFunc(Func &&f,Args && ...args) {
    //  return Func(std::bind(std::forward<Func>(f),std::forward<Args>(args)...));
    return std::bind(std::forward<Func>(f),std::forward<Args>(args)...);
  }
  

}


#endif
