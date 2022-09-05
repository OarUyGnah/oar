#include "Semaphore.h"

#include <stdexcept>

namespace oar {

  Sem::Sem(int count) {
    if(sem_init(&_sem, 0, count)) {
      throw std::logic_error("Sem::sem() error");
    }
  }
  
  Sem::~Sem() {
    sem_destroy(&_sem);
  }

  void Sem::wait() {
    if(sem_wait(&_sem)) {
      throw std::logic_error("Sem::wait() error");
    }
  }

  void Sem::post() {
    if(sem_post(&_sem)) {
      throw std::logic_error("Sem::post() error");
    }
  }
}
