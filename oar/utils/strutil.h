#ifndef __OAR_STRUTIL_H__
#define __OAR_STRUTIL_H__

#include <string.h>
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

    // 把string和const char*的进一步封装
    // 返回第几次出现的位置，如果不到则返回size()值，并将最后一次出现的位置存储在max中
    template<typename T = std::string>
    size_t findNth(std::string &str,T c,size_t nth,size_t *max = nullptr) {
      if(nth <= 0) return str.size();
      size_t size = str.size();
      *max = size;
      int pos = 0;
      int idx = 0;
      int len = c.size();
      for_each(begin(str),end(str),[&](char &cc) {
	if(idx+len <= str.size() && !strncmp(&str[idx],c.c_str(),len)) {
	  if(nth > 0) {
	    --nth;
	    if(max) {
	      pos = idx;
	      *max = pos;
	    }
	  }
	}
	++idx;
      });
      return nth == 0 ? pos : size;
    
    }
    

    template<>
    inline size_t findNth<const char*>(std::string &str,const char* c,size_t nth,size_t *max) {
      if(nth < 0) return str.size();
      size_t size = str.size();
      *max = size;
      int pos = 0;
      int idx = 0;
      int len = strlen(c);
      for_each(begin(str),end(str),[&](char &cc) {
	if(idx+len <= str.size() && !strncmp(&str[idx],c,len)) {
	  if(nth > 0) {
	    --nth;
	    if(max) {
	      pos = idx;
	      *max = pos;
	    }
	  }
	}
	++idx;
      });
      return nth == 0 ? pos : size;
    }
    

    template<>
    inline size_t findNth<char>(std::string &str,char c,size_t nth,size_t *max) {
      if(nth < 0) return str.size();
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

    

    template<typename T = std::string>
    inline size_t rfindNth(std::string &str,T t,size_t nth) {
      //      return _rfindNth(str,t,nth)
      if(nth <= 0) return str.size();
      size_t pos = std::string::npos;
      while(nth--) {
	pos = str.rfind(t,pos-1);
	if(pos < 0 || pos > str.size()) {
	  return str.size();
	}	
      }
      return pos;
    }
    
    template<>
    inline size_t rfindNth<char>(std::string &str,char c,size_t nth) {
      if(nth <= 0) return str.size();
      int pos = str.size() - 1;
      int idx = pos;
      //      printf("begin pos %d\n",pos);
      for_each(rbegin(str),rend(str),[&](char &cc) {
	if(cc == c) {
	  if(nth > 0) {
	    --nth;
	    pos = idx;
	    //    printf("current pos is %d , char is %c\n",pos,cc);
	  }
	}
	--idx;
      });
      //      printf("pos is %d nth is %d\n",pos,nth);
      return nth == 0 ? pos : str.size();
    }
    
    template<>
    inline size_t rfindNth<const char*>(std::string &str,const char* c,size_t nth) {
      if(nth <= 0) return str.size();
      size_t pos = std::string::npos;
      while (nth--) {
	pos = str.rfind(c,pos-1);
	if(pos < 0 || pos > str.size()) {
	  return str.size();
	}
	//	printf("current pos is %d\n",pos);
      }
      return pos;
    }

    
    } // namespace strutil
} // namespace oar
#endif
