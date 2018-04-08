#pragma once

#include <string>
#include <map>
#include <mutex>


#ifdef _WIN32
  #define __FUNCTION_SIGNATURE__ __FUNCSIG__
#else
  #define __FUNCTION_SIGNATURE__ __PRETTY_FUNCTION__
#endif

#define LOG_VERBOSE(str) xmol::utils::Logger::Log((str), (xmol::utils::Logger::VERBOSE),0 )
#define LOG_TRACE(str) xmol::utils::Logger::Log((str), (xmol::utils::Logger::TRACE),0 )
#define LOG_DEBUG(str) xmol::utils::Logger::Log((str), (xmol::utils::Logger::DEBUG),0 )
#define LOG_ERROR(str) xmol::utils::Logger::Log((str), (xmol::utils::Logger::_ERROR), 0 )
#define LOG_WARNING(str) xmol::utils::Logger::Log((str), (xmol::utils::Logger::WARNING),0 )

#define LOG_TRACE_FUNCTION()  xmol::utils::Logger::DummyEnterFunctionLog DummyEnterFunctionLog_(__FUNCTION_SIGNATURE__,__FILE__,xmol::utils::Logger::TRACE)
#define LOG_DEBUG_FUNCTION()  xmol::utils::Logger::DummyEnterFunctionLog DummyEnterFunctionLog_(__FUNCTION_SIGNATURE__,__FILE__,xmol::utils::Logger::DEBUG)
#define LOG_VERBOSE_FUNCTION() xmol::utils::Logger::DummyEnterFunctionLog DummyEnterFunctionLog_(__FUNCTION_SIGNATURE__,__FILE__,xmol::utils::Logger::VERBOSE)

#define LOG_TRACE_SCOPE(str)   xmol::utils::Logger::DummyEnterScopeLog DummyEnterScopeLog_((str),__FILE__,__LINE__,xmol::utils::Logger::TRACE)
#define LOG_DEBUG_SCOPE(str)   xmol::utils::Logger::DummyEnterScopeLog DummyEnterScopeLog_((str),__FILE__,__LINE__,xmol::utils::Logger::DEBUG)
#define LOG_VERBOSE_SCOPE(str) xmol::utils::Logger::DummyEnterScopeLog DummyEnterScopeLog_((str),__FILE__,__LINE__,xmol::utils::Logger::VERBOSE)



namespace xmol::utils {

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
