#include "log.h"
#include "./utils/strutil.h"
#include "Mutex.h"
#include <algorithm>
#include <bits/types/struct_tm.h>
#include <cerrno>
#include <cstddef>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <new>
#include <tuple>
#include <utility>
#include <vector>

namespace oar {
const char *LogLevel::toString(LogLevel::Level level) {
  switch (level) {
#define JUDGE(name)                                                            \
  case LogLevel::name:                                                         \
    return #name;                                                              \
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

LogLevel::Level LogLevel::fromString(const std::string &str) {

#define JUDGE(level, v)                                                        \
  if (str == #v) {                                                             \
    return LogLevel::level;                                                    \
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

LogEvent::LogEvent(loggerPtr logger, logLevel level, const char *filename,
                   int32_t line, uint32_t elapse, uint32_t threadId,
                   TimeStamp ts, const std::string &threadName)
    : _filename(filename), _line(line), _elapse(elapse), _threadId(threadId),
      _ts(ts), _threadName(threadName), _ss(), _level(level), _logger(logger) {
  // std::cout << "threadid == " << this->threadId() << std::endl;
}

void LogEvent::printlog(const char *fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  printlog(fmt, vl);
  va_end(vl);
}
void LogEvent::printlog(const char *fmt, va_list vl) {
  char *buf = NULL;
  int len = 0;
  if ((len = vasprintf(&buf, fmt, vl)) != -1) {
    _ss << std::string(buf, len);
    free(buf);
  }
}

LogFormatter::LogFormatter(const std::string &pattern, bool isError)
    : _pattern(pattern), _error(isError) {
  initFormatter();
}

class MessageItem : public LogFormatter::Item {
public:
  MessageItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << event->content();
  }
};

class LogLevelItem : public LogFormatter::Item {
public:
  LogLevelItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << LogLevel::toString(level);
  }
};

class ElaspeItem : public LogFormatter::Item {
public:
  ElaspeItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << event->elaspe();
  }
};
// 日志器名
class LoggerNameItem : public LogFormatter::Item {
public:
  LoggerNameItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << event->logger()->getName();
  }
};

// 文件名
class FilenameItem : public LogFormatter::Item {
public:
  FilenameItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << event->filename();
  }
};

class LineItem : public LogFormatter::Item {
public:
  LineItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << "Line " << event->line();
  }
};
// TODO
class TimeItem : public LogFormatter::Item {
public:
  TimeItem(const std::string &str = "%Y-%m-%d %H:%M:%S") : _format(str) {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    struct tm tm;
    time_t time = event->ts().totime_t();
    localtime_r(&time, &tm);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), _format.c_str(), &tm);
    os << timebuf;
  }

private:
  std::string _format;
};

class NewLineItem : public LogFormatter::Item {
public:
  NewLineItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << std::endl;
  }
};

class StringFmtItem : public LogFormatter::Item {
public:
  StringFmtItem(const std::string &str) : _str(str) {}

  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << _str;
  }

private:
  std::string _str;
};

class TabItem : public LogFormatter::Item {
public:
  TabItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << "\t";
  }
};

class ThreadIdItem : public LogFormatter::Item {
public:
  ThreadIdItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << event->threadId();
  }
};

class ThreadNameItem : public LogFormatter::Item {
public:
  ThreadNameItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << event->threadName();
  }
};

class ColonItem : public LogFormatter::Item {
public:
  ColonItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << ":";
  }
};

class LSquareBracketItem : public LogFormatter::Item {
public:
  LSquareBracketItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << "[";
  }
};

class RSquareBracketItem : public LogFormatter::Item {
public:
  RSquareBracketItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << "]";
  }
};

class SpaceItem : public LogFormatter::Item {
public:
  SpaceItem(const std::string &str = "") {}
  void format(std::ostream &os, LogFormatter::loggerPtr logger,
              LogLevel::Level level, LogEvent::eventPtr event) {
    os << " ";
  }
};

void LogFormatter::initFormatter() {
  // static char[] charset = {
  //     'm', // 消息
  //     'p', // 日志级别
  //     'r', // 毫秒
  //     'c', // 日志器名
  //     'F', // 文件名
  //     't', // tab
  //     'n', // 换行
  //     'T', // 线程id
  //     'l', // 行号
  //     'N', // 线程名称
  //     'd'  // 日期
  // };
  // std::cout << _pattern << std::endl;
  using pos_t = size_t;
  // bool 是否正确 char 字符  pos_t字符位置 std::string args
  std::vector<std::tuple<bool, char, pos_t, std::string>> vec;
  // std::cout << _pattern.size() << std::endl;
  for (int i = 0; i < _pattern.size(); ++i) {
    if (_pattern[i] == '%') {
      i++;
      // if (!have(_pattern[i])) {
      //   continue;
      // }
      switch (_pattern[i]) {
      case 'm':
      case 'p':
      case 'r':
      case 'c':
      case 'F':
      case 't':
      case 'n':
      case 'T':
      case 'l':
      case 'N':
        vec.emplace_back(true, _pattern[i], i, "");
        break;
      case 'd':
        // 时间格式
        ++i;
        if (_pattern[i] == '{' || _pattern[i] == '}') {
          if (_pattern[i] == '{') {
            ++i;
            auto pos = oar::strutil::findFirst(_pattern.substr(i), '}');
            std::string str = _pattern.substr(i, pos);
            // std::cout << str << std::endl;
            vec.emplace_back(true, 'd', i - 1, str);
            i += pos;
          } else {
            vec.emplace_back(false, _pattern[i], i, "");
            _error = true;
          }
        } else {
          vec.emplace_back(false, _pattern[i], i, "");
          _error = true;
        }
        break;

      default:
        vec.emplace_back(false, _pattern[i], i, "");
        _error = true;
        break;
      }
    } else {
      switch (_pattern[i]) {
      case ':':
        std::cout << ":" << std::endl;
        vec.emplace_back(true, _pattern[i], i, "");
        break;
      case '[':
        vec.emplace_back(true, _pattern[i], i, "");
        break;
      case ']':
        vec.emplace_back(true, _pattern[i], i, "");
        break;
      case ' ':
        vec.emplace_back(true, _pattern[i], i, "");
        break;
      default:
        break;
      }
    }
  }
  static std::map<char, std::function<Item::itemPtr(const std::string &str)>>
      items_map = {
#define XX(str, C)                                                             \
  {                                                                            \
    str, [](const std::string &fmt) { return Item::itemPtr(new C(fmt)); }      \
  }

          XX('m', MessageItem),        // m:消息
          XX('p', LogLevelItem),       // p:日志级别
          XX('r', ElaspeItem),         // r:累计毫秒数
          XX('c', LoggerNameItem),     // c:日志器名
          XX('F', FilenameItem),       // f:文件名
          XX('t', TabItem),            // t:tab
          XX('n', NewLineItem),        // n:换行
          XX('T', ThreadIdItem),       // T:线程id
          XX('l', LineItem),           // l:行号
          XX('N', ThreadNameItem),     // N:线程名
          XX('d', TimeItem),           // d:日期时间
          XX(':', ColonItem),          // 冒号
          XX('[', LSquareBracketItem), // 左方括号
          XX(']', RSquareBracketItem), // 右方括号
          XX(' ', SpaceItem)           // 空格
#undef XX
      };
  // for (auto &tuple : vec) {
  //   std::cout << std::get<0>(tuple) << " " << std::get<1>(tuple) << " "
  //             << std::get<2>(tuple) << " " << std::get<3>(tuple) <<
  //             std::endl;
  // }
  for (auto &tuple : vec) {
    if (std::get<0>(tuple)) {
      auto it = items_map.find(std::get<1>(tuple));
      if (it == items_map.end()) {
        goto error;
        // char errorfmt = std::get<1>(tuple);
        // _items.push_back(Item::itemPtr(new StringFmtItem(
        //     "<<error_format %" + std::string(errorfmt, 1) + ">>")));
      } else {
        _items.push_back(it->second(std::get<3>(tuple)));
      }
    } else {
    error:
      _items.push_back(Item::itemPtr(new StringFmtItem(
          "<<error_format %" + std::string(std::get<1>(tuple), 1) + ">>")));
    }
  }
  int times = 0;
  for (auto &i : _items) {
    std::cout << ++times << std::endl;
  }
}

// void LogFormatter::initFormatter() {
//   std::vector<std::tuple<std::string, std::string, int>> vec;
//   std::string nstr;
//   for (int i = 0; i < _pattern.size(); i++) {
//     if (_pattern[i] != '%') {
//       nstr.append(1, _pattern[i]);
//       continue;
//     }
//     if ((i + 1) < _pattern.size()) {
//       if (_pattern[i + 1] == '%') {
//         nstr.append(1, '%');
//         continue;
//       }
//     }
//     int idx = i + 1;
//     int format_stat = 0;
//     int format_begin = 0;
//     std::string str;
//     std::string fmt;
//     while (idx < _pattern.size()) {
//       if (!format_stat && _pattern[idx] != '{' && _pattern[idx] != '}' &&
//           !isalpha(_pattern[idx])) {
//         str = _pattern.substr(i + 1, idx - i - 1);
//         break;
//       }
//       if (!format_stat) {
//         if (_pattern[idx] == '{') {
//           str = _pattern.substr(i + 1, idx - i - 1);
//           format_stat = 1;
//           format_begin = idx;
//           ++idx;
//           continue;
//         }
//       } else if (format_stat == 1) {
//         if (_pattern[idx] == '}') {
//           fmt = _pattern.substr(format_begin + 1, idx - format_begin - 1);
//           format_stat = 0;
//           ++idx;
//           break;
//         }
//       }
//       ++idx;
//       if (idx == _pattern.size()) {
//         if (str.empty()) {
//           str = _pattern.substr(i + 1);
//         }
//       }
//     }
//     if (!format_stat) {
//       if (!nstr.empty()) {
//         vec.push_back(std::make_tuple(nstr, std::string(), 0));
//         nstr.clear();
//       }
//       vec.push_back(std::make_tuple(nstr, fmt, 1));
//       i = idx - 1;
//     } else if (format_stat == 1) {
//       std::cerr << "pattern parse error: " << _pattern << "-"
//                 << _pattern.substr(i) << std::endl;
//       _error = true;
//       vec.push_back(std::make_tuple("<<pattern error>>", fmt, 0));
//     }
//   }
//   if (!nstr.empty()) {
//     vec.push_back(std::make_tuple(nstr, "", 0));
//   }
//   static std::map<std::string,
//                   std::function<Item::itemPtr(const std::string &str)>>
//       s_format_items = {
// #define XX(str, C) \
//   { \
// #str, [](const std::string &fmt) { return Item::itemPtr(new C(fmt)); }     \
//   }

//           XX(m, MessageItem),  // m:消息
//           XX(p, LogLevelItem), // p:日志级别
//           // XX(r, ElapseFormatItem),            //r:累计毫秒数
//           // XX(c, NameFormatItem),              //c:日志名称
//           // XX(t, ThreadIdFormatItem),          //t:线程id
//           XX(n, NewLineItem),  // n:换行
//           XX(d, TimeItem),     // d:时间
//           XX(f, FilenameItem), // f:文件名
//           XX(l, LineItem),     // l:行号
//           XX(T, TabItem),      // T:Tab
//   // XX(N, ThreadNameFormatItem),        //N:线程名称
// #undef XX
//       };
//   for (auto &item : vec) {

//     if (std::get<2>(item) == 0) {
//       std::cout << "item is" << std::get<0>(item) << std::endl;
//       _items.push_back(Item::itemPtr(new StringFmtItem(std::get<0>(item))));
//     } else {
//       auto it = s_format_items.find(std::get<0>(item));
//       if (it == s_format_items.end()) {
//         _items.push_back(Item::itemPtr(
//             new StringFmtItem("<<error_format %" + std::get<0>(item) +
//             ">>")));
//         _error = true;
//       } else {
//         _items.push_back(it->second(std::get<1>(item)));
//       }
//     }
//   }
// }

std::string LogFormatter::format(loggerPtr logger, LogLevel::Level level,
                                 LogEvent::eventPtr event) {
  std::stringstream ss;
  for (auto &item : _items) {
    item->format(ss, logger, level, event);
  }
  return ss.str();
}
std::ostream &LogFormatter::format(std::ostream &ofs, loggerPtr logger,
                                   LogLevel::Level level,
                                   LogEvent::eventPtr event) {
  for (auto &item : _items) {
    item->format(ofs, logger, level, event);
  }
  return ofs;
}

void LogAppender::setFormatter(LogFormatter::formatterPtr fmt) {
  MutexGuard mg(_mutex);
  _formatter = fmt;
  if (_formatter) {
    _hasFormatter = true;
  } else
    _hasFormatter = false;
}

LogFormatter::formatterPtr LogAppender::getFormatter() {
  MutexGuard mg(_mutex);
  return _formatter;
}

void FileAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                       LogEvent::eventPtr event) {
  if (level >= _level) {
    uint64_t now = event->ts().getmicroEpoch();
    if (now >= (_lastTime + 3)) {
      reopen();
      _lastTime = now;
    }
    MutexGuard mg(_mutex);
    if (!_formatter->format(_ofs, logger, level, event)) {
      std::cerr << "error FileAppender::log" << std::endl;
    }
  }
}

bool FileAppender::reopen() {
  MutexGuard mg(_mutex);
  if (_ofs) {
    _ofs.close();
  }
  return FSUtil::openForWrite(_ofs, _filename, std::ios::app);
}

void StdoutAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                         LogEvent::eventPtr event) {
  if (level >= _level) {
    _formatter->format(std::cout, logger, level, event);
  }
}

Logger::Logger(const std::string &name)
    : _name(name), _level(LogLevel::DEBUG),
      _formatter(LogFormatter::formatterPtr(new LogFormatter)), _mainLogger() {}

void Logger::log(LogLevel::Level level, LogEvent::eventPtr event) {
  if (level >= _level) {
    auto self = shared_from_this();
    MutexGuard mg(_mutex);
    if (!_appenders.empty()) {
      for (auto &appender : _appenders) {
        appender->log(self, level, event);
      }
    } else if (_mainLogger) {
      // TODO
      // if (_mainLogger == nullptr) {
      //   std::cout << "_mainlogger == nullptr" << std::endl;
      // }
      _mainLogger->log(level, event);
    }
    // std::cout << "end Logger::log" << std::endl;
  }
}
void Logger::debug(LogEvent::eventPtr event) { log(LogLevel::DEBUG, event); }
void Logger::info(LogEvent::eventPtr event) { log(LogLevel::INFO, event); }
void Logger::warn(LogEvent::eventPtr event) { log(LogLevel::WARN, event); }
void Logger::error(LogEvent::eventPtr event) { log(LogLevel::ERROR, event); }
void Logger::fatal(LogEvent::eventPtr event) { log(LogLevel::FATAL, event); }

void Logger::addAppender(LogAppender::appendPtr appender) {
  MutexGuard mg(_mutex);
  // 如果添加的appender没有formatter，就用Logger的
  if (!appender->getFormatter()) {
    // MutexGuard mgInner(appender->_mutex);
    appender->_formatter = _formatter;
  }
  _appenders.push_back(appender);
}
void Logger::rmAppender(LogAppender::appendPtr appender) {
  MutexGuard mg(_mutex);
  _appenders.erase(std::remove_if(
      _appenders.begin(), _appenders.end(),
      [&](LogAppender::appendPtr &ptr) { return ptr == appender; }));
}
void Logger::cleanAllAppender() {
  MutexGuard mg(_mutex);
  _appenders.clear();
}

void Logger::setFormatter(LogFormatter::formatterPtr formatter) {
  MutexGuard smg(_mutex);
  for (auto &appender : _appenders) {
    if (!appender->_hasFormatter) {
      appender->setFormatter(formatter);
    }
  }
  _formatter = formatter;
}
void Logger::setFormatter(const std::string &val) {
  LogFormatter::formatterPtr newVal(new LogFormatter(val));
  if (newVal->isError()) {
    std::cerr << "Logger setFormatter name=" << _name << " value=" << val
              << " invalid formatter" << std::endl;
    return;
  }
  setFormatter(newVal);
}

LoggerManager::LoggerManager() {
  _mainLogger.reset(new Logger);
  _mainLogger->addAppender(LogAppender::appendPtr(new StdoutAppender));
  _loggers.insert({_mainLogger->getName(), _mainLogger});
}

bool LoggerManager::insert(const std::string &name, Logger::LoggerPtr ptr) {
  return _loggers.insert({name, ptr}).second;
}

Logger::LoggerPtr LoggerManager::getLogger(const std::string &name) {
  auto it = _loggers.find(name);
  if (it != _loggers.end()) {
    return it->second;
  } else {
    Logger::LoggerPtr ret(new Logger(name));
    ret->_mainLogger = _mainLogger;
    _loggers.insert({name, ret});
    return ret;
  }
}

} // namespace oar
