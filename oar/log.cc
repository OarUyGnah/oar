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

  LogEvent::LogEvent(loggerPtr logger, logLevel level, const char* name,
		     int32_t line, uint32_t elapse, uint32_t threadId,
		     TimeStamp ts, const std::string& threadName)
    :_filename(name),
     _line(line),
     _elapse(elapse),
     _threadId(threadId),
     _ts(ts),
     _threadName(threadName),
     _level(level),
     _logger(logger)
  {}


  
  void LogEvent::printlog(const char* fmt,...) {
    va_list vl;
    va_start(vl, fmt);
    printlog(fmt, vl);
    va_end(vl);
  }
  void LogEvent::printlog(const char* fmt,va_list vl) {
    char* buf = NULL;
    int len = 0;
    if ((len = vasprintf(&buf, fmt, vl)) != -1) {
      _ss << std::string(buf,len);
      free(buf);
    }
  }

  
}
