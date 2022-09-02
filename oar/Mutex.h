#ifndef __OAR_MUTEX_H__
#define __OAR_MUTEX_H__

#include "./utils/noncopyable.h"
#include "ThisThread.h"
#include <pthread.h>
#include <assert.h>

namespace oar {

  class Mutex : Noncopyable {
  public:
    
    Mutex() : _holderTid(0) {
      pthread_mutex_init(&_mutex,nullptr);
    }
    
    ~Mutex() {
      pthread_mutex_destroy(&_mutex);  
    }
    
    void lock() {
      pthread_mutex_lock(&_mutex);
      setTid();
    }
    
    void unlock() {
      clearTid();
      pthread_mutex_unlock(&_mutex);
    }

    void setTid() {
      _holderTid = ThisThread::tid();
    }
    
    void clearTid() {
      _holderTid = 0;
    }

    bool LockedByThisThread() {
      return _holderTid == ThisThread::tid();
    }
    
    pthread_mutex_t* getMutexPtr() {
      return &_mutex;
    }
    
  private:

    pthread_mutex_t _mutex;
    // 表示给互斥量上锁线程的tid
    pid_t _holderTid;
    
  };

  /*
    为条件变量 pthread_cond_wait()调用时设计的
   */
  class HolderTidGuard : Noncopyable {
  public:
    
    HolderTidGuard(Mutex& m) : _mutex(m) {
      _mutex.clearTid();
    }
    
    ~HolderTidGuard() {
      _mutex.setTid();
    }

  private:
    Mutex& _mutex;
  };
  
  class MutexGuard : Noncopyable{
  public:
    
    MutexGuard(Mutex& m) : _mutex(m) {
      _mutex.lock();
    }
    
    ~MutexGuard() {
      _mutex.unlock();
    }
    
  private:
    
    Mutex& _mutex;
    
  };
}




#endif
