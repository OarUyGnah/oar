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


  string s2 = "1234565432123";
  cout << s2 <<"strutil::findFirst(string,'1') == " << strutil::findFirst(s2,'1') << endl;
  cout << s2 <<"strutil::findFirst(string,\"321\") == " << strutil::findFirst(s2,"321") << endl;
  cout << s2 <<"strutil::findFirst(string,string(\"456\")) == " << strutil::findFirst(s2,string("456")) << endl;
    cout << s2 <<"strutil::findLast(string,'1') == " << strutil::findLast(s2,'1') << endl;
      cout << s2 <<"strutil::findLast(string,\"123\") == " << strutil::findLast(s2,"123") << endl;
  cout << s2 <<"strutil::findLast(string,string(\"456\")) == " << strutil::findLast(s2,string("456")) << endl;

  
  cout << "===================stdutil::findNth===================" << endl;
  size_t pos = 0;
  cout << "strutil::findNth(s2, '1', 2, &pos) == " << strutil::findNth(s2, '1', 2, &pos) << " last pos is " << pos << endl;
  cout << "strutil::findNth(s2, '1', 2, &pos) == " << strutil::findNth(s2, '1', 2, &pos) << " last pos is " << pos << endl;
  cout << "strutil::findNth(s2, '1', 3, &pos) == " << strutil::findNth(s2, '1', 3, &pos) << " last pos is " << pos << endl;
  cout << "strutil::findNth(s2, '3', 4, &pos) == " << strutil::findNth(s2, '3', 4, &pos) << " last pos is " << pos << endl;
  
  return 0;
}
