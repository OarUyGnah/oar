#ifndef __OAR_THREAD_H__
#define __OAR_THREAD_H__
#include <functional>
#include <memory>
#include <pthread.h>

namespace oar {
  class Thread {
    typedef std::function<void()> Func;
  private:
    pthread_t _pthreadId;
    Func _func;
    std::string _name;
    pid_t _pid;
    bool joined;
    bool started;
  public:
    Thread(std::function<void()> func,const std::string& name = std::string());
    ~Thread();
    bool joinable() { return !joined; }
    bool started() { return started; }
    int join();
    pid_t pid() { return _pid; }
    
  };

}


#endif
