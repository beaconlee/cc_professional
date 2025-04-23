#pragma once

#include <glog/logging.h>

namespace logger
{

// 自动初始化类
class Logger
{
public:
  static void
  Init(const char *program_name)
  {
    static Logger log{program_name};
  }

private:
  Logger(const char *program_name);

};

#define INFO LOG(INFO)
#define WARNING LOG(WARNING)
#define ERROR LOG(ERROR)
#define FATAL LOG(FATAL)


} // namespace logger
