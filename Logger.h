#pragma once

#include <string>
#include <map>
#include <mutex>


#ifdef _WIN32
  #define __FUNCTION_SIGNATURE__ __FUNCSIG__
#else
  #define __FUNCTION_SIGNATURE__ __PRETTY_FUNCTION__
#endif

#define LOG_VERBOSE(str) Common::Logger::Log((str), (Common::Logger::VERBOSE),0 )
#define LOG_TRACE(str) Common::Logger::Log((str), (Common::Logger::TRACE),0 )
#define LOG_DEBUG(str) Common::Logger::Log((str), (Common::Logger::DEBUG),0 )
#define LOG_ERROR(str) Common::Logger::Log((str), (Common::Logger::_ERROR), 0 )
#define LOG_WARNING(str) Common::Logger::Log((str), (Common::Logger::WARNING),0 )

#define LOG_TRACE_FUNCTION()  Common::Logger::DummyEnterFunctionLog DummyEnterFunctionLog_(__FUNCTION_SIGNATURE__,__FILE__,Common::Logger::TRACE)
#define LOG_DEBUG_FUNCTION()  Common::Logger::DummyEnterFunctionLog DummyEnterFunctionLog_(__FUNCTION_SIGNATURE__,__FILE__,Common::Logger::DEBUG)
#define LOG_VERBOSE_FUNCTION() Common::Logger::DummyEnterFunctionLog DummyEnterFunctionLog_(__FUNCTION_SIGNATURE__,__FILE__,Common::Logger::VERBOSE)

#define LOG_TRACE_SCOPE(str)   Common::Logger::DummyEnterScopeLog DummyEnterScopeLog_((str),__FILE__,__LINE__,Common::Logger::TRACE)
#define LOG_DEBUG_SCOPE(str)   Common::Logger::DummyEnterScopeLog DummyEnterScopeLog_((str),__FILE__,__LINE__,Common::Logger::DEBUG)
#define LOG_VERBOSE_SCOPE(str) Common::Logger::DummyEnterScopeLog DummyEnterScopeLog_((str),__FILE__,__LINE__,Common::Logger::VERBOSE)



namespace Common {

class Logger
{
public:
  enum LOG_LEVEL
  {
    VERBOSE,
    DEBUG,
    TRACE,
    WARNING,
    _ERROR
  };
  Logger(const Logger& root) = delete ;
  Logger& operator=(const Logger&) = delete;

  static void init(const std::string filename, LOG_LEVEL log_lvl);
  static void Log(const std::string& log_info, LOG_LEVEL log_lvl, int indent_inc=0);

  class DummyEnterFunctionLog;
  class DummyEnterScopeLog;

  /* to complain C++-.2003 */
  friend class DummyEnterFunctionLog;
  friend class DummyEnterScopeLog;

private:
  unsigned indent;
  std::map<LOG_LEVEL, std::string> level_names;
  LOG_LEVEL log_level;
  std::string logfile;
  std::mutex mutex;
  static Logger& instance();
  Logger(){};
};

class Logger::DummyEnterFunctionLog
{
const char* sign;
LOG_LEVEL lvl;
public:
  DummyEnterFunctionLog(const char* _sign, const char* file, LOG_LEVEL _lvl);
  ~DummyEnterFunctionLog();
};

class Logger::DummyEnterScopeLog
{
  const char* sign;
  LOG_LEVEL lvl;
public:
  DummyEnterScopeLog(const char* _sign, const char* file, int line, LOG_LEVEL _lvl);
  ~DummyEnterScopeLog();
};

}
