

#include "strutil.h"
#include <stdio.h>
#include <algorithm>
#include <string.h>
namespace oar {
  
  namespace strutil {

    
    void deleteLeftChar(std::string &str,const char c) {
      if (!str.size() && str.empty()) return;
      auto it = find_if(str.begin(),str.end(),
			[=](char &cc) { return c!=cc; });
      str = str.substr(it-begin(str));
    }
    
    void deleteLeftChar(char *str,const char c) {
      std::string s(str);
      auto it = find_if(s.begin(),s.end(),
			[=](char &cc) { return c!=cc; });
      strcpy(str,s.c_str()+(it-begin(s)));
      str[s.size()] = '\0';
    }

    void deleteRightChar(std::string &str,const char c) {
      if (!str.size() && str.empty()) return;
      // Reverse::base()  reverse iterator -> normal iterator
      auto it = find_if(str.rbegin(),str.rend(),[=](char &cc) { return c!=cc; }).base();
      str = str.substr(0, it - begin(str));
    }

    void deleteRightChar(char *str,const char c) {
      std::string s(str);
      auto it = find_if(s.rbegin(),s.rend(),[=](char &cc) { return c!=cc; }).base();
      s.erase(it, end(s));
      strcpy(str,s.c_str());
    }
    
    
  }
}


