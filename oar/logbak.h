// #ifndef __OAR_LOG_H__
// #define __OAR_LOG_H__

#include "Time.h"
#include "Mutex.h"
#include "utils/FileUtil.h"
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdarg.h>
#include <vector>

namespace oar {

  class Logger;
  
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

  class LogEvent {
  public:
    using eventPtr = std::shared_ptr<LogEvent> ;
    using loggerPtr = std::shared_ptr<oar::Logger>;
    using eventStream = std::stringstream;
    using logLevel = LogLevel::Level;

    LogEvent() = default;
    LogEvent(loggerPtr logger, logLevel level, const char* name,
	     int32_t line, uint32_t elapse, uint32_t threadId,
	     TimeStamp ts, const std::string& threadName);
    
    const char* filename() const { return _filename; }
    int32_t line() const { return _line; }
    uint32_t elaspe() const { return _elapse; }
    uint32_t threadId() const { return _threadId; }
    const std::string& threadName() const { return _threadName; }
    std::string content() const { return _ss.str(); }
    loggerPtr logger() const { return _logger; }
    eventStream& stream() { return _ss; }
    LogLevel::Level level() const { return _level; }
    TimeStamp ts() const { return _ts; }
    // 格式化写入日志
    void printlog(const char* fmt,...);
    void printlog(const char* fmt,va_list vl);
    
    
  private:
    const char* _filename;
    int32_t _line;
    uint32_t _elapse; // 程序启动到现在的毫秒数
    uint32_t _threadId;
    TimeStamp _ts;
    std::string _threadName;
    eventStream _ss;
    logLevel _level;
    loggerPtr _logger;
  };

  /// @brief 
  class LogFormatter {
  public:
    using formatterPtr = std::shared_ptr<LogFormatter>;
    using loggerPtr = std::shared_ptr<Logger>;
    
    class Item {
    public:
      using itemPtr = std::shared_ptr<Item>;
      Item() {}
      virtual ~Item() {}
      virtual void format(std::ostream& os, loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) = 0;
    };

    
    LogFormatter(const std::string& pattern = "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%T[%p]%T[%c]%T%f:%l%T%m%n");
    //void init();
    
    std::string format(loggerPtr logger, LogLevel::Level level,
                        LogEvent::eventPtr event);
    std::ostream& format(std::ostream& ofs, loggerPtr logger,
                        LogLevel::Level level, LogEvent::eventPtr event);


    void initFormatter();
    
    bool isError() const { return _error; }
    
    void setPattern(std::string pattern) { _pattern = pattern; }
    const std::string& getPattern() const { return _pattern; }
  private:
    std::string _pattern;
    std::vector<Item::itemPtr> _items;
    bool _error = false;
  };

  class LogAppender {
  public:
    using appendPtr = std::shared_ptr<LogAppender>;
    using MutexType = SpinMutex;
    
    virtual ~LogAppender() {
      
    }

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
		     LogEvent::eventPtr event) = 0;
    void setFormatter(LogFormatter::formatterPtr fmt);

    LogFormatter::formatterPtr getFormatter();

    LogLevel::Level level() const {
      return _level;
    }
    void setLevel(LogLevel::Level level) {
      _level = level;
    }
  protected:
    LogLevel::Level _level = LogLevel::DEBUG;
    MutexType _mutex;
    LogFormatter::formatterPtr _formatter;
    bool _hasFormatter = false;
  };

  class FileAppender : public LogAppender {
  public:
    using fileAppenderPtr = std::shared_ptr<FileAppender>;

    FileAppender(std::string filename) : _filename(filename) {
      std::cout << "open test" << std::endl;
    }

    void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
	     LogEvent::eventPtr event);

    bool reopen();
    
  private:
    std::ofstream _ofs;
    std::string _filename;
    uint64_t _lastTime;
  };
  
  
}


// #endif
