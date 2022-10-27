// #ifndef __OAR_LOG_H__
// #define __OAR_LOG_H__

#include "Mutex.h"
#include "Singleton.h"
#include "Time.h"
#include "utils/FileUtil.h"
#include <fstream>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdarg.h>
#include <string>
#include <vector>

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_LEVEL(logger, level)                                               \
  if (logger->getLevel() <= level)                                             \
  oar::LogEventWrapper(                                                        \
      LogEvent::eventPtr(new LogEvent(logger, level, __FILENAME__, __LINE__,   \
                                      0, ThisThread::tid(), TimeStamp(),       \
                                      ThisThread::threadName)))                \
      .getStream()

#define LOG_DEBUG(logger) LOG_LEVEL(logger, oar::LogLevel::DEBUG)
#define LOG_INFO(logger) LOG_LEVEL(logger, oar::LogLevel::INFO)
#define LOG_WARN(logger) LOG_LEVEL(logger, oar::LogLevel::WARN)
#define LOG_ERROR(logger) LOG_LEVEL(logger, oar::LogLevel::ERROR)
#define LOG_FATAL(logger) LOG_LEVEL(logger, oar::LogLevel::FATAL)

#define LOG_FORMAT(logger, level, format, ...)                                 \
  if (logger->getLevel() <= level)                                             \
  oar::LogEventWrapper(                                                        \
      LogEvent::eventPtr(new LogEvent(logger, level, __FILENAME__, __LINE__,   \
                                      0, ThisThread::tid(), TimeStamp(),       \
                                      ThisThread::threadName)))                \
      .getEvent()                                                              \
      ->printlog(format, __VA_ARGS__)

#define LOG_FORMAT_DEBUG(logger, format, ...)                                  \
  LOG_FORMAT(logger, oar::LogLevel::DEBUG, format, __VA_ARGS__)
#define LOG_FORMAT_INFO(logger, format, ...)                                   \
  LOG_FORMAT(logger, oar::LogLevel::INFO, format, __VA_ARGS__)
#define LOG_FORMAT_WARN(logger, format, ...)                                   \
  LOG_FORMAT(logger, oar::LogLevel::WARN, format, __VA_ARGS__)
#define LOG_FORMAT_ERROR(logger, format, ...)                                  \
  LOG_FORMAT(logger, oar::LogLevel::ERROR, format, __VA_ARGS__)
#define LOG_FORMAT_FATAL(logger, format, ...)                                  \
  LOG_FORMAT(logger, oar::LogLevel::FATAL, format, __VA_ARGS__)
namespace oar {

class Logger;
class LoggerManager;

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

  static const char *toString(LogLevel::Level level);

  static LogLevel::Level fromString(const std::string &str);
};

class LogEvent {
public:
  using eventPtr = std::shared_ptr<LogEvent>;
  using loggerPtr = std::shared_ptr<Logger>;
  using eventStream = std::stringstream;
  using logLevel = LogLevel::Level;

  LogEvent() = default;
  LogEvent(loggerPtr logger, logLevel level, const char *name, int32_t line,
           uint32_t elapse, uint32_t threadId, TimeStamp ts,
           const std::string &threadName);

  const char *filename() const { return _filename; }
  int32_t line() const { return _line; }
  uint32_t elaspe() const { return _elapse; }
  uint32_t threadId() const { return _threadId; }
  const std::string &threadName() const { return _threadName; }
  std::string content() const { return _ss.str(); }
  std::shared_ptr<Logger> logger() const { return _logger; }
  eventStream &stream() { return _ss; }
  LogLevel::Level level() const { return _level; }
  TimeStamp ts() const { return _ts; }
  // 格式化写入日志
  void printlog(const char *fmt, ...);
  void printlog(const char *fmt, va_list vl);

private:
  const char *_filename;   // 文件名
  int32_t _line;           // 行号
  uint32_t _elapse;        // 程序启动到现在的毫秒数
  uint32_t _threadId;      // 线程id
  TimeStamp _ts;           // 时间戳
  std::string _threadName; // 线程名
  eventStream _ss;         // 事件流
  logLevel _level;         // 日志等级
  loggerPtr _logger;       // 日志器
};

class LogFormatter {
public:
  using formatterPtr = std::shared_ptr<LogFormatter>;
  using loggerPtr = std::shared_ptr<Logger>;

  class Item {
  public:
    using itemPtr = std::shared_ptr<Item>;
    Item() {}
    virtual ~Item() {}
    virtual void format(std::ostream &os, loggerPtr logger,
                        LogLevel::Level level, LogEvent::eventPtr event) = 0;
  };

  LogFormatter(const std::string &pattern =
                   "[%p]%t%d{%Y-%m-%d %H:%M:%S} [%T:%N] [%c]%t%F : %l%t%m%n",
               bool isError = false);
  // void init();

  std::string format(loggerPtr logger, LogLevel::Level level,
                     LogEvent::eventPtr event);
  std::ostream &format(std::ostream &ofs, loggerPtr logger,
                       LogLevel::Level level, LogEvent::eventPtr event);

  void initFormatter();

  bool isError() const { return _error; }

  void setPattern(std::string pattern) { _pattern = pattern; }
  const std::string &getPattern() const { return _pattern; }

  bool have(char c) {
    for (auto cc : charset) {
      if (cc == c) {
        return true;
      }
    }
    return false;
  }

private:
  std::string _pattern;
  std::vector<Item::itemPtr> _items;
  bool _error = false;

public:
#ifdef __cplusplus &&__cplusplus >= 201703
  constexpr
#endif
      static char charset[] = {
          'm', // 消息
          'p', // 日志级别
          'r', // 毫秒
          'c', // 日志名称
          'F', // 文件名称
          't', // tab
          'n', // 换行
          'T', // 线程id
          'l', // 行号
          'N', // 线程名称
          'd'  // 日期
  };
};

class LogAppender {
  friend class Logger;

public:
  using appendPtr = std::shared_ptr<LogAppender>;
  using MutexType = Mutex;

  virtual ~LogAppender() {}

  virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                   LogEvent::eventPtr event) {}
  void setFormatter(LogFormatter::formatterPtr fmt);

  LogFormatter::formatterPtr getFormatter();

  LogLevel::Level level() const { return _level; }
  void setLevel(LogLevel::Level level) { _level = level; }

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

class StdoutAppender : public LogAppender {
public:
  using StdoutAppenderPtr = std::shared_ptr<StdoutAppender>;
  // StdoutAppender() = default;
  void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
           LogEvent::eventPtr event);
};

class Logger : public std::enable_shared_from_this<Logger> {
  friend class LoggerManager;

public:
  using LoggerPtr = std::shared_ptr<Logger>;
  Logger(const std::string &name = "mainLogger");
  void init() { _mainLogger = shared_from_this(); }
  void log(LogLevel::Level level, LogEvent::eventPtr event);
  void debug(LogEvent::eventPtr event);
  void info(LogEvent::eventPtr event);
  void warn(LogEvent::eventPtr event);
  void error(LogEvent::eventPtr event);
  void fatal(LogEvent::eventPtr event);

  void addAppender(LogAppender::appendPtr appender);
  void rmAppender(LogAppender::appendPtr appender);
  void cleanAllAppender();

  LogLevel::Level getLevel() const { return _level; }
  void setLevel(LogLevel::Level level) { _level = level; }

  const std::string &getName() const { return _name; }

  void setFormatter(LogFormatter::formatterPtr formatter);
  void setFormatter(const std::string &val);
  LogFormatter::formatterPtr getFormatter() {
    MutexGuard mg(_mutex);
    return _formatter;
  }

private:
  std::string _name;
  LogLevel::Level _level;
  Mutex _mutex;
  std::list<LogAppender::appendPtr> _appenders;
  LogFormatter::formatterPtr _formatter;
  Logger::LoggerPtr _mainLogger; // 主日志器
};

class LoggerManager {
public:
  LoggerManager();
  Logger::LoggerPtr getLogger(const std::string &name);
  Logger::LoggerPtr getMainLogger() { return _mainLogger; }
  bool insert(const std::string &name, Logger::LoggerPtr ptr);
  void init() {
    // _mainLogger.reset(new Logger);
    // _mainLogger->init();
    // _mainLogger->addAppender(LogAppender::appendPtr(new
    // StdoutAppender));
    // //_mainLogger->addAppender(std::make_shared<StdoutAppender>());
    // _loggers.insert({_mainLogger->getName(), _mainLogger});
  }

private:
  Mutex _mutex;
  std::map<std::string, Logger::LoggerPtr> _loggers;
  Logger::LoggerPtr _mainLogger;
};

class LogEventWrapper {
public:
  using wrapperPtr = std::shared_ptr<LogEventWrapper>;
  LogEventWrapper(LogEvent::eventPtr event) : _event(event) {}

  ~LogEventWrapper() {
    auto logger = _event->logger();
    // TODO 此处loggerptr为nullptr
    logger->log(_event->level(), _event);
  }

  // void writelog() { _event->logger()->log(_event->level(), _event); }

  LogEvent::eventPtr getEvent() const { return _event; }

  std::stringstream &getStream() { return _event->stream(); }

private:
  LogEvent::eventPtr _event;
};

extern oar::SingletonPtr<oar::LoggerManager> LoggerMgr;
} // namespace oar

// #endif
