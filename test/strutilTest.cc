#include "../oar/utils/strutil.h"
#include <iostream>
#include <string.h>
using namespace std;
using namespace oar;

int main() {
  vector<string> vec;
  
  strutil::strSplit("1231232 213 12321321 3",vec,string(" "));
  //strutil::strSplit("1231232 213 12321321 3",vec,' ');
    
  for(auto &s:vec) {
    cout << s << endl;
  }
      
  
  char str[32] = "  begin..!@.ab  cd..$%.end  ";
  printf("sizeof str is %d\n",strlen(str));
  string s(str);
  strutil::deleteLeftChar(s,' ');
  strutil::deleteLeftChar(str, ' ');
  cout << "after deleteleftchar string s is |" << s << "|" <<endl;
  cout << "after deleteleftchar char* str is |" << str << "|" <<endl;

  strutil::deleteRightChar(s,' ');
  strutil::deleteRightChar(str, ' ');
  cout << "after deleterightchar string s is |" << s << "|" <<endl;
  cout << "after deleterightchar char* str is |" << str << "|" <<endl;


  return 0;
}
