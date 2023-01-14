#ifndef __OAR_CONDITION_H__
#define __OAR_CONDITION_H__

#include "oar/Mutex.h"
#include "oar/utils/noncopyable.h"
#include <errno.h>
namespace oar {

class Condition : Noncopyable {
public:
    Condition(Mutex& m)
        : _mutex(m)
    {
        pthread_cond_init(&_cond, nullptr);
    }

    ~Condition()
    {
        pthread_cond_destroy(&_cond);
    }

    void wait()
    {
        HolderTidGuard htg(_mutex);
        pthread_cond_wait(&_cond, _mutex.getMutexPtr());
    }

    void notify()
    {
        pthread_cond_signal(&_cond);
    }

    void broadcast()
    {
        pthread_cond_broadcast(&_cond);
    }

    bool timedWait(double seconds);

private:
    Mutex& _mutex;
    pthread_cond_t _cond;
};

inline bool Condition::timedWait(double seconds)
{
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    constexpr int64_t nanosecondsPerSecond = 1000000000;
    int64_t nanoseconds = static_cast<int64_t>(seconds * nanosecondsPerSecond);

    time.tv_sec += static_cast<time_t>((time.tv_sec + nanoseconds) / nanosecondsPerSecond);
    time.tv_nsec = static_cast<long>((time.tv_sec + nanoseconds) % nanosecondsPerSecond);

    HolderTidGuard htg(_mutex);

    return ETIMEDOUT == pthread_cond_timedwait(&_cond, _mutex.getMutexPtr(), &time);
}
}

#endif
