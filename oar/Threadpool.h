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
  inline void defaultThreadpoolInitCallBack() {
    printf("defaultThreadpoolInitCallBack\n"); // test
  }
  class Threadpool {
  public:
    using ThreadVec = std::vector<std::unique_ptr<oar::Thread>>;
    using Task = std::function<void()>;
    using TaskQueue = std::deque<Task>;

    Threadpool(int threadNums, const std::string name = std::string("Threadpool"), Task task = defaultThreadpoolInitCallBack);
    //    Threadpool();
    ~Threadpool();

    void start();
    void stop();
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
    Task _initCallBack;

  };
  
}


#endif
