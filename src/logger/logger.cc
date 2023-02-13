#include "logger.hpp"

#include <iomanip>

/* This function writes a prefix that matches glog's default format.
 * (The third parameter can be used to receive user-supplied data, and is
 * NULL by default.)
 */
static void CustomLogPrefix(std::ostream &s, const google::LogMessageInfo &l,
                            void *);

/* This function writes a prefix that matches glog's default format.
 * (The third parameter can be used to receive user-supplied data, and is
 * NULL by default.)
 */
void CustomLogPrefix(std::ostream &s, const google::LogMessageInfo &l,
                            void *) {
  s << "[HelloApp]"
    << '[' 
    << std::setw(4) << 1900 + l.time.year() << '-'
    << std::setw(2) << 1 + l.time.month() << '-' 
    << std::setw(2) << l.time.day()
    << ' ' 
    << std::setw(2) << l.time.hour() << ':' 
    << std::setw(2) << l.time.min() << ':' 
    << std::setw(2) << l.time.sec() 
    << ']'
    << '[' << l.filename << ':' << l.line_number << ']'
    << '[' << l.severity << ']';
    // << std::setw(10 - strlen(l.severity)) << std::setfill(' ');
}

void Logger_Init(const char* app) {
  // 设置仅向stderr输出日志
  FLAGS_logtostderr = 1;
  // 设置向stderr输出颜色日志
  FLAGS_colorlogtostderr = 1;
  // 控制输出日志的最小等级， INFO = 0, WARNING = 1, ERROR = 2
  FLAGS_minloglevel = google::INFO;
  google::InitGoogleLogging(app, &CustomLogPrefix);
}