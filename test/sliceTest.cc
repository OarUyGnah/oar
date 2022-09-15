#include "../oar/utils/strutil.h"
#include <iostream>
#include <algorithm>

using namespace oar;
using namespace std;

int main () {
  string s = "1234567890";
  strutil::Slice sl(s.c_str(),s.size());
  cout << "string s = " << s << endl;
  cout << "Slice sl = " << sl << "sl.size() = " << sl.size() <<endl;
  cout << *sl.begin() << endl;
  cout << "sl[2] = " << sl[2] << endl;
  string s2 = "abcdefg";
  sl.setData(s2.c_str());
  cout << "after setData sl = " << sl << endl;
  sl.setData(s2.c_str(),s2.size());
  cout << "after setData sl = " << sl << endl;
  cout << find_if(sl.begin(),sl.end(),[](char c) { return c == 'f'; }) - sl.begin() << endl;
  //  sl[0] = 'a';
  strutil::Slice sl2 = s2;
  cout << "sl2 = " << sl2 << " sl2.size() = " << sl2.size() << endl;
  cout << "sl == sl2 ? " << (sl == sl2) << endl;
  //cout << "after sl[0] = 'a' Slice sl = " << sl << endl;
  sl2 = std::move(string("stdmove..."));
  cout << "sl2 = " << sl2 << endl;
  sl2 = s;
  cout << "sl2 = " << sl2 << endl;

  sl2.remove_prefix(3);
  cout << "after sl2.remove_prefix(3) sl2 = " << sl2 << " sl2.size() = " << sl2.size() << endl;
  sl2.remove_suffix(2);
  cout << "after sl2.remove_suffix(2) sl2 = " << sl2 << " sl2.size() = " << sl2.size() << endl;
  return 0;
}
