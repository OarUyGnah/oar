#ifndef __OAR_SEMAPHORE_H__
#define __OAR_SEMAPHORE_H__

#include "oar/utils/noncopyable.h"

#include <semaphore.h>

namespace oar {

class Sem : Noncopyable {
public:
    Sem(int count = 0);

    ~Sem();

    void wait();

    void post();

private:
    sem_t _sem;
};

}

#endif
