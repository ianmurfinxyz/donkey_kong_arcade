#ifndef DONKEY_KONG_1981_LOG_HH
#define DONKEY_KONG_1981_LOG_HH

#include <string>

enum class log_lvl
{
  fatal,
  error,
  warning,
  info
};

constexpr const char* fmt_fatal {"\e[1;31m"};
constexpr const char* fmt_error {"\e[1;31m"};
constexpr const char* fmt_warning {"\e[1;33m"};
constexpr const char* fmt_info {"\e[1;36m"};
constexpr const char* fmt_bold {"\e[1m"};
constexpr const char* fmt_clear {"\e[0m"};

void log(log_lvl lvl, const std::string& msg);
void log(log_lvl lvl, const char* msg);

#endif //DONKEY_KONG_1981_LOG_HH
