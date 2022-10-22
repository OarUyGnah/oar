#include "log.h"
#include "./utils/strutil.h"
#include <map>
#include <tuple>
#include <iostream>

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

  LogFormatter::LogFormatter(const std::string& pattern)
  :_pattern(pattern) {
    initFormatter();
  }


  class MessageItem : public LogFormatter::Item {
  public:
    MessageItem(const std::string& str = "") {}
    void format(std::ostream& os, LogFormatter::loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << event->content();
    }
  };

  class LogLevelItem : public LogFormatter::Item {
  public:
    LogLevelItem(const std::string& str = "") {}
    void format(std::ostream& os, LogFormatter::loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << LogLevel::toString(level);
    }
  };

  class FilenameItem : public LogFormatter::Item {
  public:
    FilenameItem(const std::string& str = "") {}
    void format(std::ostream& os, LogFormatter::loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << event->filename();
    }
  };

  class LineItem : public LogFormatter::Item {
  public:
    LineItem(const std::string& str = "") {}
    void format(std::ostream& os, LogFormatter::loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << event->line();
    }
  };
  // TODO
  class TimeItem : public LogFormatter::Item {
  public:
    TimeItem(const std::string& str = "") {}
    void format(std::ostream& os, LogFormatter::loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << event->ts();
    }
  };

  class NewLineItem : public LogFormatter::Item {
  public:
    NewLineItem(const std::string& str = "") {}
    void format(std::ostream& os, LogFormatter::loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << std::endl;
    }
  };

  class StringFmtItem : public LogFormatter::Item {
  public:
    StringFmtItem(const std::string& str) : _str(str) {}
    
    void format(std::ostream& os, LogFormatter::loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << _str;
    }
  private:
    std::string _str;
  };

  class TabItem : public LogFormatter::Item {
  public:
    TabItem(const std::string& str = "") {}
    void format(std::ostream& os, LogFormatter::loggerPtr logger, LogLevel::Level level, LogEvent::eventPtr event) {
      os << "\t";
    }
  };



  void LogFormatter::initFormatter() {
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for (int i = 0; i < _pattern.size(); i++) {
      if (_pattern[i] != '%') {
        nstr.append(1, _pattern[i]);
        continue;
      }
      if ((i + 1) < _pattern.size()) {
        if(_pattern[i + 1] == '%') {
          nstr.append(1, '%');
          continue;
        }
      }
      int idx = i + 1;
      int format_stat = 0;
      int format_begin = 0;
      std::string str;
      std::string fmt;
      while (idx < _pattern.size()) {
        if (!format_stat && _pattern[idx] != '{'
            && _pattern[idx] != '}' && !isalpha(_pattern[idx])) {
              str = _pattern.substr(i + 1, idx - i - 1);
              break;
        }
        if (!format_stat) {
          if (_pattern[idx] == '{') {
            str = _pattern.substr(i + 1, idx - i - 1);
            format_stat = 1;
            format_begin = idx;
            ++idx;
            continue;
          }
        } else if (format_stat == 1) {
          if (_pattern[idx] == '}') {
            fmt = _pattern.substr(format_begin + 1, idx - format_begin - 1);
            format_stat = 0;
            ++idx;
            break;
          }
        }
        ++idx;
        if (idx == _pattern.size()) {
          if (str.empty()) {
            str = _pattern.substr(i + 1);
          }
        }
      }
      if (!format_stat) {
        if (!nstr.empty()) {
          vec.push_back(std::make_tuple(nstr, std::string(), 0));
          nstr.clear();
        }
        vec.push_back(std::make_tuple(nstr, fmt, 1));
        i = idx - 1;
      } else if (format_stat == 1) {
        std::cerr << "pattern parse error: " << _pattern << "-" 
        << _pattern.substr(i) << std::endl;
        _error = true;
        vec.push_back(std::make_tuple("<<pattern error>>", fmt, 0));
      }
      
      
    }
    if (!nstr.empty()) {
      vec.push_back(std::make_tuple(nstr, "", 0));
    }
    static std::map<std::string, std::function<Item::itemPtr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return Item::itemPtr(new C(fmt));}}

        XX(m, MessageItem),               //m:消息
        XX(p, LogLevelItem),              //p:日志级别
        // XX(r, ElapseFormatItem),            //r:累计毫秒数
        // XX(c, NameFormatItem),              //c:日志名称
        // XX(t, ThreadIdFormatItem),          //t:线程id
        XX(n, NewLineItem),               //n:换行
        XX(d, TimeItem),                  //d:时间
        XX(f, FilenameItem),              //f:文件名
        XX(l, LineItem),                  //l:行号
        XX(T, TabItem),                   //T:Tab
        // XX(N, ThreadNameFormatItem),        //N:线程名称
#undef XX
    };
    for(auto& item : vec) {

      if(std::get<2>(item) == 0) {
        std::cout << "item is" <<std::get<0>(item) << std::endl;
        _items.push_back(Item::itemPtr(new StringFmtItem(std::get<0>(item))));
      } else {
        auto it = s_format_items.find(std::get<0>(item));
        if (it == s_format_items.end()) {
          _items.push_back(Item::itemPtr(new StringFmtItem("<<error_format %" + std::get<0>(item) + ">>")));
          _error = true;
        } else {
          _items.push_back(it->second(std::get<1>(item)));
        }
      }
    }
  }



  std::string LogFormatter::format(loggerPtr logger, LogLevel::Level level,
                        LogEvent::eventPtr event) {
    std::stringstream ss;
    for(auto& item : _items) {
      item->format(ss, logger, level, event);
    }
    return ss.str();
  }
  std::ostream& LogFormatter::format(std::ostream& ofs, loggerPtr logger,
                        LogLevel::Level level, LogEvent::eventPtr event) {
    for(auto& item : _items) {
      item->format(ofs, logger, level, event);
    }
    return ofs;
  }

  
  
  


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
      uint64_t now = event->ts().getmicroEpoch();
      if (now >= (_lastTime + 3)) {
	reopen();
	_lastTime = now;
      }
      SpinMutexGuard smg(_mutex);
      /*      if (!_formatter->format(_ofs, logger, level, event)) {
	std::cerr << "error FileAppender::log" << std::endl;
      }
      */
    }
  }
  
  bool FileAppender::reopen() {
    SpinMutexGuard spg(_mutex);
    if (_ofs) {
      _ofs.close();
    }
    return FSUtil::openForWrite(_ofs, _filename, std::ios::app);
  }
  
  
}
