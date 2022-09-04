#include <functional>
#include <iostream>

using namespace std;
namespace oar {
class Test {
public:
  using Func = std::function<void()>;
  
  template<typename F,typename ...Args>
  Test(F&& f,Args&& ...args){
    cout << sizeof...(args) << endl;
    _f = std::bind(std::forward<F>(f),std::forward<Args>(args)...);
  }
  void run() {
    _f();
  }
  Func _f;
};
}
void func1(int a,int b) {
  cout << a << " " << b <<endl;
}


int main() {
  oar::Test t1([](int a){cout << a <<endl;},200);
  t1.run();

  oar::Test t2(func1,200,300);
  t2.run();
  
  return 0;
}
