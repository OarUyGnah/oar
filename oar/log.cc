#include "log.h"
#include "./utils/strutil.h"

namespace oar {
  const char* LogLevel::toString(LogLevel::Level level) {
    switch(level) {
#define JUDGE(name)				\
      case LogLevel::name:			\
        return #name;				\
        break;

      JUDGE(DEBUG);
      JUDGE(INFO);
      JUDGE(WARN);
      JUDGE(ERROR);
      JUDGE(FATAL);
#undef JUDGE
    default:
      return "UNKNOWN";
    }
    return "UNKNOWN";
  }

  LogLevel::Level LogLevel::fromString(const std::string& str) {
    
#define JUDGE(level, v)				\
    if(str == #v) {				\
      return LogLevel::level;			\
    }
    
    JUDGE(DEBUG, debug);
    JUDGE(INFO, info);
    JUDGE(WARN, warn);
    JUDGE(ERROR, error);
    JUDGE(FATAL, fatal);

    JUDGE(DEBUG, DEBUG);
    JUDGE(INFO, INFO);
    JUDGE(WARN, WARN);
    JUDGE(ERROR, ERROR);
    JUDGE(FATAL, FATAL);
    return LogLevel::UNKNOWN;
#undef JUDGE
  }
  
}
