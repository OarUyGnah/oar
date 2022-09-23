#include "../../oar/log.h"
#include <iostream>

using namespace oar;
using namespace std;

int main() {
  LogEvent le;
  le.printlog("what are you doing ? %d %c\n", 12,'a');
  cout << le.content();
  return 0;
}
