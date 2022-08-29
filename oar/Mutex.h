#ifndef __OAR_MUTEX_H__
#define __OAR_MUTEX_H__

#include "./utils/noncopyable.h"
#include <pthread.h>
#include <assert.h>
namespace oar {

  class Mutex : oar::Noncopyable {
  public:
    
    Mutex() : _holder(0) {
      pthread_mutex_init(&_mutex,nullptr);
    }
    
    ~Mutex() {
      pthread_mutex_destroy(&_mutex);  
    }
    
    void lock() {
      pthread_mutex_lock(&_mutex);
    }
    
    void unlock() {
      pthread_mutex_unlock(&_mutex);
    }
    
    pthread_mutex_t* getMutexPtr() {
      return &_mutex;
    }
    
  private:
    
    pthread_mutex_t _mutex;
    pid_t _holder;
    
  };

  class MutexGuard : oar::Noncopyable{
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
