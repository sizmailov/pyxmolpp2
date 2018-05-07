#include "xmol/utils/Logger.h"
#include <iostream>
#include <fstream>
#include <ctime>

void xmol::utils::Logger::init(std::string filename, xmol::utils::Logger::LOG_LEVEL log_lvl) {
  instance().level_names[VERBOSE] = "[VERBOSE]";
  instance().level_names[DEBUG] = "[ DEBUG ]";
  instance().level_names[TRACE] = "[ TRACE ]";
  instance().level_names[WARNING] = "[WARNING]";
  instance().level_names[_ERROR] = "[ ERROR ]";
  instance().log_level = log_lvl;
  instance().logfile = std::move(filename);
  instance().indent = 0;

  std::fstream log(instance().logfile, std::ios::out | std::ios::app);
  std::time_t t = std::time(nullptr);
  char mbstr[30];
  std::string time_stamp;
  if (std::strftime(mbstr, sizeof(mbstr), "[ %F %T ]", std::localtime(&t)) > 0) {
    time_stamp = mbstr;
  }
  else {
    time_stamp = "[ no time stamp ]";
  }

  log
  << time_stamp
  << "[ INIT  ]"
  << " "
  << std::endl;
  log.flush();
}
void xmol::utils::Logger::Log(const std::string& log_info, xmol::utils::Logger::LOG_LEVEL log_lvl,
                              int indent_inc/*=0*/) {

  if (log_lvl >= instance().log_level) {
    std::fstream log(instance().logfile, std::ios::out | std::ios::app);

    if (!log.good()) {
      // todo throw
    }

    if (indent_inc < 0) {
      instance().indent += indent_inc;
    }
    std::string sindent(std::max(0, instance().indent), ' ');
    if (indent_inc > 0) {
      instance().indent += indent_inc;
    }

    std::time_t t = std::time(nullptr);
    char mbstr[30];
    std::string time_stamp;
    if (std::strftime(mbstr, sizeof(mbstr), "[ %F %T ]", std::localtime(&t)) > 0) {
      time_stamp = mbstr;
    }
    else {
      time_stamp = "[ no time stamp ]";
    }

    log
    << time_stamp
    << instance().level_names[log_lvl].c_str()
    << " "
    << sindent
    << log_info.c_str()
    << std::endl;

    if (log_lvl >= WARNING) {
      std::cerr
      << time_stamp
      << instance().level_names[log_lvl].c_str()
      << " "
      << sindent
      << log_info.c_str()
      << std::endl;

    }
  }
}
xmol::utils::Logger& xmol::utils::Logger::instance() {
  static Logger logger;
  return logger;
}
xmol::utils::Logger::DummyEnterFunctionLog::~DummyEnterFunctionLog() {
  if (lvl >= Logger::instance().log_level) {
    Logger::Log("} // Leaving function  '" + std::string(sign) + "'", (lvl), -2);
  }
}
xmol::utils::Logger::DummyEnterFunctionLog::DummyEnterFunctionLog(const char* _sign, const char* file,
                                                                  xmol::utils::Logger::LOG_LEVEL _lvl) : sign(_sign)
                                                                                                         , lvl(_lvl) {
  if (_lvl >= Logger::instance().log_level) {
    Logger::Log(std::string(sign) + " { // file:" + std::string(file), (lvl), +2);
  }
}
xmol::utils::Logger::DummyEnterScopeLog::DummyEnterScopeLog(const char* _sign, const char* file, int line,
                                                            xmol::utils::Logger::LOG_LEVEL _lvl) : sign(_sign), lvl(
    _lvl) {
  if (_lvl >= Logger::instance().log_level) {
    Logger::Log(
        "<named scope '" + std::string(sign) + "'> { // file:" + std::string(file)+":"+std::to_string(line),
        (lvl), +2);
  }
}
xmol::utils::Logger::DummyEnterScopeLog::~DummyEnterScopeLog() {
  if (lvl >= Logger::instance().log_level) {
    Logger::Log("} // Leaving  '" + std::string(sign) + "'", (lvl), -2);
  }
}
