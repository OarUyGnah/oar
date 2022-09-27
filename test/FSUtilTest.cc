#include "../oar/utils/FileUtil.h"
#include <iostream>

using namespace oar;
using namespace std;

int main() {
  
  vector<string> files;
  FSUtil::listAllFile(files,".",".cc");
  cout << "---listAllFile----" << endl;
  for (auto &s : files) {
    cout << s << endl;
  }

  return 0;

}
