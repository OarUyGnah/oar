#ifndef __OAR_NONCOPYABLE_H__
#define __OAR_NONCOPYABLE_H__
namespace oar{
  
  class Noncopyable {
  private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
  protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
  };

}

#endif
