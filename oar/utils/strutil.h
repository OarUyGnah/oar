#ifndef __OAR_STRUTIL_H__
#define __OAR_STRUTIL_H__

#include <string>
#include <vector>
#include <algorithm>

namespace oar {

  namespace strutil {
    
    // 要加inline否则会有multiple definition的问题
    inline void _strSplit(std::string &str, std::vector<std::string> &v, std::string spacer) {
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
								     
    // inline
    template<typename T = std::string>
    inline void strSplit(std::string str, std::vector<std::string> &v, T spacer)
    {
      _strSplit(str,v,spacer);
    }

    template<>
    inline void strSplit(std::string str, std::vector<std::string> &v, const char* spacer)
    {
      _strSplit(str,v,std::string(spacer));
    }
											   
    
    template<>
    inline void strSplit(std::string str, std::vector<std::string> &v, char spacer)
    {
      _strSplit(str,v,std::string(1,spacer));
    }
    

    void deleteLeftChar(std::string &str,const char c);
    void deleteLeftChar(char *str,const char c);

    void deleteRightChar(std::string &str,const char c);
    void deleteRightChar(char *str,const char c);

    //void trim(std::string &str,bool left,bool right,const char *rm);

    
    template<typename T = const char*>
    size_t findFirst(std::string str,T c) {
      return str.find(c);
    }
    template<typename T = const char*>
    size_t findLast(std::string str,T c) {
      return str.rfind(c);
    }

    template<typename T = const char*>
    size_t findNth(std::string &str,T c,size_t nth,size_t *max = nullptr) {
      size_t times = 0;
      /*      find_if(begin(str),end(str),[&times](T &c) ->bool {
	if(c == )
	});*/
    }
    

    // 返回第几次出现的位置，如果不到则返回size()值，并将最后一次出现的位置存储在max中,from1即从1开始
    template<>
    inline size_t findNth(std::string &str,char c,size_t nth,size_t *max) {
      size_t times = 0,last = 0;
      auto resIt = find_if(begin(str),end(str),[&](char &curr) ->bool {
	if(c == curr) {
	  ++times;
	  if(max)
	    *max = last;
	}
	++last;
	return times == nth ? true : false;
      });
      return static_cast<size_t>(resIt-begin(str));
    }


    template<typename T>
    inline size_t rfindNth(T t) {
      return 0;
    }
    
    
  } // namespace strutil
} // namespace oar
#endif
