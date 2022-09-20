#include "../../oar/log.h"
#include "../../oar/utils/strutil.h"
#include <iostream>

using namespace oar;
using namespace std;

int main() {
  auto level = LogLevel::fromString(string("DEBUG"));
  cout << "level = " << level << endl;
  string s = "123!@#abcDEF";
  strutil::upperCase(s);
  
  cout << s << endl;
  return 0;
}
