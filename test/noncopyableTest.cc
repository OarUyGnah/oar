#include "../oar/utils/noncopyable.h"
#include <iostream>


class test : oar::Noncopyable {
  
};

int main() {
  test t;
  test t1(t);
}


