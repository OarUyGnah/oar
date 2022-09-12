#ifndef __OAR_SINGLETON_H__
#define __OAR_SINGLETON_H__

#include <memory>

namespace oar {


  // 返回裸指针
  template<class T,class X = void,int N = 0>
  class Singleton {
  public:
    static T* getInstance() {
      static T t;
      return &t;
    }
  };
  // 返回shared_ptr
  // T类型 X为创造多个实例对应的Tag N同一个Tag创造多个实例索引  
  template<class T,class X = void,int N = 0>
  class SingletonPtr {
  public:
    static std::shared_ptr<T> getInstance() {
      static std::shared_ptr<T> ptr(new T);
      return ptr;
    }
  };


}


#endif
