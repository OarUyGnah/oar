#ifndef __OAR_BLOCKINGQUEUE_H__
#define __OAR_BLOCKINGQUEUE_H__

#include "Condition.h"

#include <deque>

namespace oar {

  template<typename T,class Vec = std::deque<T>>
  class BlockingQueue : Noncopyable {
  public:
    

    BlockingQueue()
      :_mutex(),
       _cond(_mutex),
       _queue()
    {
    }

    void push(const T& t) {
      MutexGuard mg(_mutex);
      _queue.push_back(t);
      _cond.notify();
    }

    void push(T&& t) {
      MutexGuard mg(_mutex);
      _queue.push_back(std::move(t));
      _cond.notify();
    }

    T take() {
      MutexGuard mg(_mutex);
      while(_queue.empty()) {
	_cond.wait();
      }
      assert(!_queue.empty());
      T t(std::move(_queue.front()));
      _queue.pop_front();
      return t;
    }
    
    size_t size() const {
      MutexGuard mg(_mutex);
      return _queue.size();
    }

    Vec takeall() {
      Vec vec;
      {
	MutexGuard mg(_mutex);
	vec = std::move(_queue);
	assert(vec.empty());
      }
    }
    
  private:
    Mutex _mutex;
    Condition _cond;
    Vec _queue;
  };


  
}


#endif
