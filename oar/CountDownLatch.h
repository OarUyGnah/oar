#ifndef __OAR_COUNTDOWNLATCH_H__
#define __OAR_COUNTDOWNLATCH_H__

#include "Condition.h"
//#include "Mutex.h"

namespace oar {

  class CountDownLatch : Noncopyable {
  public:

    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

  private:
    mutable Mutex _mutex;
    Condition _cond;
    int _count;
  };
}


#endif
