#ifndef __OAR_THREADPOOL_H__
#define __OAR_THREADPOOL_H__

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"
#include "Types.h"

#include <vector>
#include <deque>
#include <stdio.h>

namespace oar {
  inline void defaultThreadpoolInitCallBack(std::string &s) {
    printf("Threadpool name:%s initialization completed\n",s.c_str()); // test
  }
  class Threadpool {
  public:
    using ThreadVec = std::vector<std::unique_ptr<oar::Thread>>;
    using Task = std::function<void()>;
    using TaskQueue = std::deque<Task>;
    using ThreadpoolInitCb = std::function<void(std::string&)>;
    Threadpool(int threadNums, const std::string name = std::string("Threadpool"), ThreadpoolInitCb task = defaultThreadpoolInitCallBack);
    //    Threadpool();
    ~Threadpool();

    void start();
    void stop();
    void stopRunNewTask();
    void runTask(Task task);

    
    const std::string& name() const { return _name; }
    
    size_t taskQueuesize() const;
    void setTaskQueueSize(size_t size);
    
  private:
    void loopInThread();
    void everyThreadJoin();
    bool isFull() const;
    Task take();

  private:
    size_t _threadNums;
    size_t _maxTaskSize;
    std::string _name;
    mutable Mutex _mutex;
    Condition _notfull;
    Condition _notempty;
    ThreadVec _threads;
    TaskQueue _tasks;
    bool _running;
    bool _stopRunNewTask;
    std::function<void(std::string&)> _initCallBack;
    
  };
  
}


#endif
