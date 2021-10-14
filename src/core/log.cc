#include <iostream>
#include "log.hh"

static void print_level(log_lvl lvl)
{
  switch(lvl){
    case log_lvl::fatal:
      std::cout << fmt_fatal << "fatal: " << fmt_clear;
      break;
    case log_lvl::error:
      std::cout << fmt_error << "error: " << fmt_clear;
      break;
    case log_lvl::warning:
      std::cout << fmt_warning << "warning: " << fmt_clear;
      break;
    case log_lvl::info:
      std::cout << fmt_info << "info: " << fmt_clear;
      break;
  }
}

void log(log_lvl lvl, const std::string& msg)
{
  print_level(lvl);
  std::cout << msg << std::endl;
}

void log(log_lvl lvl, const char* msg){
  print_level(lvl);
  std::cout << msg << std::endl;
}
