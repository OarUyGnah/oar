#ifndef __OAR_STRUTIL_H__
#define __OAR_STRUTIL_H__

#include <string>
#include <vector>

namespace oar {

  namespace strutil {
    
    
    void _strSplit(std::string &str, std::vector<std::string> &v, std::string spacer) {
      int pos1, pos2;
      int len = spacer.length(); //记录分隔符的长度
      pos1 = 0;
      pos2 = str.find(spacer);
      printf("pos2 is %d\n",pos2);
      while (pos2 != std::string::npos)
	{
	  v.push_back(str.substr(pos1, pos2 - pos1));
	  pos1 = pos2 + len;
	  pos2 = str.find(spacer, pos1); // 从str的pos1位置开始搜寻spacer
	}
      if (pos1 != str.length()) //分割最后一个部分
	v.push_back(str.substr(pos1));  
    }


    
    //    template<typename T = std::string>
    void strSplit(std::string &str, std::vector<std::string> &v, std::string spacer)
    {
      _strSplit(str,v,spacer);
    }

    //    template<>
    void strSplit(std::string &str, std::vector<std::string> &v, const char* spacer)
    {
      _strSplit(str,v,std::string(spacer));
    }
    
    
    //    template<>
    void strSplit(std::string &str, std::vector<std::string> &v, char spacer)
    {
      _strSplit(str,v,std::string(1,spacer));
    }
    


    void test (int i) ;
    
    
  } // namespace strutil
} // namespace oar
#endif
