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



  class MessageItem : public LogFormatter::Item {
  public:
    ~MessageItem() {

    }
    void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << event->content();
    }
  };

  class LogLevelItem : public LogFormatter::Item {
  public:
    void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << LogLevel::toString(level);
    }
  };

  class FilenameItem : public LogFormatter::Item {
  public:
    void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << event->filename();
    }
  };

  class LineItem : public LogFormatter::Item {
  public:
    void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << event->line();
    }
  };
  // TODO
  class TimeItem : public LogFormatter::Item {
  public:
    void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << event->ts();
    }
  };

  class NewLineItem : public LogFormatter::Item {
  public:
    void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << std::endl;
    }
  };

  class StringFmtItem : public LogFormatter::Item {
  public:
    StringFmtItem(const std::string& str) : _str(str) {}
    
    void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << _str;
    }
  private:
    std::string _str;
  };

  class TabItem : public LogFormatter::Item {
  public:
    void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << "\t";
    }
  };


  void LogAppender::setFormatter(LogFormatter::formatterPtr fmt) {
    SpinMutexGuard smg(_mutex);
    _formatter = fmt;
    if (_formatter) {
      _hasFormatter = true;
    } else
      _hasFormatter = false;
  }

  LogFormatter::formatterPtr LogAppender::getFormatter() {
    SpinMutexGuard smg(_mutex);
    return _formatter;
  }

  void FileAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::eventPtr event) {
    if (level >= _level) {
      uint64_t now = event->ts();
      if (now >= (_lastTime + 3)) {
	reopen();
	_lastTime = now;
      }
      SpinMutexGuard smg(_mutex);
      if (!_formatter->format(_ofs, logger, level, event)) {
	std::cerr << "error FileAppender::log" << std::endl;
      }
    }
  }
  
  bool  FileAppender::reopen() {
    SpinMutexGuard spg(_mutex);
    if (_ofs) {
      _ofs.close();
    }
    return FSUtil::openForWrite(_ofs, _filename, std::ios::app);
  }
  
  
}
