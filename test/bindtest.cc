//#include <functional>
#include <iostream>
#include <string>
#include "../oar/Thread.h"

using namespace oar;

using Func = std::function<void()>;
/*
template<typename Func,typename ...Args>
auto convertFunc(Func &&f,Args && ...args) -> decltype(std::bind(std::forward<Func>(f),std::forward<Args>(args)...)){
  //  return Func(std::bind(std::forward<Func>(f),std::forward<Args>(args)...));
  return std::bind(std::forward<Func>(f),std::forward<Args>(args)...);
}*/


int func(int arg) {
  std::cout << arg << std::endl;
}

void func2(int x) {
  std::cout << x << std::endl;
}

void func3() {
  std::cout << "func3" <<std::endl;
}

int main() {
  Func f;
  f = std::bind(func,22);
  f();
  Func f2 = std::bind(func2,2000);
  f2();

  Func f3 = convertFunc(func2,20000);
  f3();

  Thread t1([](std::string s) {
	      for(auto &c: s){
		std::cout << c <<" ";
	      }
	      std::cout <<std::endl;
	    },"12312312313");
  t1.start();
  t1.join();
  Thread t2([](std::string s) {
	      for(auto &c: s){
		std::cout << c <<" ";
	      }
	      std::cout <<std::endl;
	    },"abcdefghijklmnopqrstuvwxyz?");
  t2.start();

  
  //  getchar();
  t2.join();
  return 0;
}
