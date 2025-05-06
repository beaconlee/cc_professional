#include "log.hh"
#include <iomanip>
#include <map>


namespace logger
{

// inline void
// prefix_formatter(std::ostream &s, const google::LogMessage &m, void * /*data*/)
// {
//   s << std::left << std::setw(6) << std::setfill(' ')
//     << google::GetLogSeverityName(m.severity()) << ' ' << std::right
//     << std::setw(4) << 1900 + m.time().year() << std::setw(2)
//     << 1 + m.time().month() << std::setw(2) << m.time().day() << ' '
//     << std::setw(2) << m.time().hour() << ':' << std::setw(2) << m.time().min()
//     << ':' << std::setw(2) << m.time().sec() << ' ' << m.basename() << ':'
//     << m.line() << "]";
// }

inline void
prefix_formatter(std::ostream &s,
                 const google::LogMessage &m,
                 void * /*unused data*/)
{
  // 保存流的原始状态
  std::ios_base::fmtflags orig_flags = s.flags();
  char orig_fill = s.fill();

  // 定义日志级别到短名称的映射
  static const std::map<google::LogSeverity, std::string> severity_short_names =
      {{google::GLOG_INFO, "INFO"},
       {google::GLOG_WARNING, "WARN"},
       {google::GLOG_ERROR, "ERR"},
       {google::GLOG_FATAL, "FAT"}};

  // 获取短名称，默认为原始名称的前3个字符
  std::string severity_name = google::GetLogSeverityName(m.severity());
  auto it = severity_short_names.find(m.severity());
  if(it != severity_short_names.end())
  {
    severity_name = it->second;
  }
  else
  {
    severity_name = severity_name.substr(0, 3); // 截取前3个字符
  }

  // 格式化日志级别（左对齐，宽度4）
  s << std::left << std::setw(4) << std::setfill(' ') << severity_name << ' ';

  // 格式化时间（右对齐）
  s << std::right << std::setw(4) << 1900 + m.time().year() << std::setw(2)
    << std::setfill('0') << 1 + m.time().month() << std::setw(2)
    << std::setfill('0') << m.time().day() << ' ' << std::setw(2)
    << std::setfill('0') << m.time().hour() << ':' << std::setw(2)
    << std::setfill('0') << m.time().min() << ':' << std::setw(2)
    << std::setfill('0') << m.time().sec() << ' ';

  // 格式化文件名和行号
  s << m.basename() << ':' << m.line() << "]";

  // 恢复流的原始状态
  s.flags(orig_flags);
  s.fill(orig_fill);
}


Logger::Logger(const char *program_name)
{
  if(FLAGS_log_dir == "")
  {
    FLAGS_log_dir = "/workspace/cc_professional/data/log";
  }

  google::InitGoogleLogging(program_name);
  google::InstallPrefixFormatter(&prefix_formatter);

  std::string_view path_str(program_name);
  size_t pos = path_str.find_last_of("/\\"); // 兼容 Unix 和 Windows 路径分隔符
  std::string log_name =
      FLAGS_log_dir + "/" + std::string(path_str.substr(pos + 1));

  // 禁用 WARNING, ERROR, FATAL 的单独日志文件
  google::SetLogDestination(google::GLOG_INFO, log_name.c_str());
  google::SetLogDestination(google::GLOG_WARNING, "");
  google::SetLogDestination(google::GLOG_ERROR, "");
  google::SetLogDestination(google::GLOG_FATAL, "");

  FLAGS_minloglevel = 0;

  // 日志输出到标准输出和标准错误是互斥的
  // 设置将日志输出到 标准错误 还是 标准输出
  FLAGS_alsologtostderr = true;
  // FLAGS_colorlogtostdout = true;

  // 设置日志输出是否有颜色, 这里设置的是标准错误
  FLAGS_colorlogtostderr = true;
  // FLAGS_colorlogtostdout = true;
}


} // namespace logger