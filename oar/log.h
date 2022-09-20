#ifndef __OAR_LOG_H__
#define __OAR_LOG_H__

#include <string>

namespace oar {

  class LogLevel {
  public:
    enum Level {
      /// 未知级别
      UNKNOWN = 0,
      /// DEBUG 级别
      DEBUG = 1,
      /// INFO 级别
      INFO = 2,
      /// WARN 级别
      WARN = 3,
      /// ERROR 级别
      ERROR = 4,
      /// FATAL 级别
      FATAL = 5
    };

    static const char* toString(LogLevel::Level level);
    
    static LogLevel::Level fromString(const std::string& str);
  };

}


#endif
