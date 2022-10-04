#include "../oar/utils/FileUtil.h"
#include <iostream>
#include <unistd.h>
using namespace oar;
using namespace std;

int main() {
  

  /*
    vector<string> files;
    FSUtil::listAllFile(files,".",".cc");
    cout << "---listAllFile----" << endl;
    for (auto &s : files) {
    cout << s << endl;
    }
  */
  /*  
      FSUtil::mkdir("./mdtest/111/2");
      sleep(5);
      FSUtil::rm("./mdtest");
  */
  /*
    string rp;
    FSUtil::realpath("/home/oarrrr/myshare", rp);
    cout << rp << endl;
  */
  FSUtil::unlink("./linkargs", true);
  return 0;

}
