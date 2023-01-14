#include "oar/utils/FileUtil.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <unistd.h>
using namespace oar;
using namespace std;

int main()
{

    /*
      vector<string> files;
      FSUtil::listAllFile(files,".",".cc");
      cout << "---listAllFile----" << endl;
      for (auto &s : files) {
      cout << s << endl;
      }
    */

    // FSUtil::mkdir("./mdtest/111/2");
    // sleep(5);
    // FSUtil::rm("./mdtest");
    // ofstream ofs;
    // FSUtil::openForWrite(ofs, "22.txt", std::ios_base::app);
    // for (int i = 0; i < 200; ++i) {
    //     ofs << " i ? " << i;
    //     if (i != 199) {
    //         ofs << "\n";
    //     }
    // }
    // string rp;
    // FSUtil::realPath("/home/oarrrr", rp);
    // cout << rp << endl;

    // FSUtil::unlink("./linkargs", true);

    return 0;
}
