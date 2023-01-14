#include "oar/CountDownLatch.h"

namespace oar {

CountDownLatch::CountDownLatch(int count)
    : _mutex()
    , _cond(_mutex)
    , _count(count)
{
}

void CountDownLatch::wait()
{
    MutexGuard mg(_mutex);
    while (_count > 0) {
        _cond.wait();
    }
}

void CountDownLatch::countDown()
{
    MutexGuard mg(_mutex);
    --_count;
    if (_count == 0)
        _cond.broadcast();
}

int CountDownLatch::getCount() const
{
    return _count;
}
}
