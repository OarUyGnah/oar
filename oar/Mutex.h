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

  class RWMutex : Noncopyable {
  public:
    
    RWMutex() {
      pthread_rwlock_init(&_mutex, nullptr);
    }
    
    ~RWMutex() {
      pthread_rwlock_destroy(&_mutex);
    }

    void rdlock() {      
      pthread_rwlock_rdlock(&_mutex);
      _rdTid = ThisThread::tid();
    }

    void wrlock() {
      pthread_rwlock_wrlock(&_mutex);
      _wrTid = ThisThread::tid();
    }

    void unlock() {
      _rdTid = _wrTid = 0;
      pthread_rwlock_unlock(&_mutex);
    }
    
  private:
    
    pthread_rwlock_t _mutex;
    pid_t _rdTid;
    pid_t _wrTid;
    
  };

  class SpinMutex : Noncopyable {
  public:
    SpinMutex() {
      pthread_spin_init(&_mutex, 0);
    }
    ~SpinMutex() {
      pthread_spin_destroy(&_mutex);
    }

    void lock() {
      pthread_spin_lock(&_mutex);
    }

    void trylock() {
      pthread_spin_trylock(&_mutex);
    }
    
    void unlock() {
      pthread_spin_lock(&_mutex);
    }
    

  private:
    pthread_spinlock_t _mutex;
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

  class ReadMutexGuard : Noncopyable {
  public:
    ReadMutexGuard(RWMutex& rm) : _rmutex(rm) {
      _rmutex.rdlock();
    }
    
    ~ReadMutexGuard() {
      _rmutex.unlock();
    }
    
  private:
    
    RWMutex& _rmutex;
    
  };

  class WriteMutexGuard : Noncopyable {
  public:
    
    WriteMutexGuard(RWMutex& wm) : _wmutex(wm) {
      _wmutex.wrlock();
    }
    
    ~WriteMutexGuard() {
      _wmutex.unlock();
    }
    
  private:
    
    RWMutex& _wmutex;
     
  };

  class SpinMutexGuard : Noncopyable {
  public:
    SpinMutexGuard(SpinMutex& sm) : _mutex(sm){
      _mutex.lock();
    }
    ~SpinMutexGuard() {
      _mutex.unlock();
    }
  private:
    SpinMutex& _mutex;
  };
  
}




#endif
