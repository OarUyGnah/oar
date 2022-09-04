#include "../oar/Exception.h"
#include <iostream>

using namespace std;

void test() {
  throw oar::Exception("oar::Exception");
}

int main() {
  try {
    test();
  }
  catch(oar::Exception& ex) {
    cout << "oar" << endl;
    cout << ex.what() << endl;
    cout << ex.stackTrace() << endl;
    //    throw ex;
  }
  catch(std::exception& ex) {
    cout << "std" << endl;
    cout << ex.what() << endl;
  }
  

}
