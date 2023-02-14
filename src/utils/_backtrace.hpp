#pragma once

#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <iomanip>
#include <string>

#include "logger/logger.hpp"

namespace utils {

#define BACKTRACE_STACK_MAX_SIZE (16)

class Backtrace {
 private:
  Backtrace() { signal(SIGSEGV, SignalHandler); }

  static void SignalHandler(int signo) {
    std::string cmd("cat /proc/self/maps");
    cmd.replace(cmd.find("self"), std::string("self").length(),
                std::to_string(getpid()));
    system(cmd.c_str());
    LOG(WARNING) << "===========>>>Catch Signal " << signo << "<<<===========";
    LOG(WARNING) << "dump stack begin...";
    [] {
      void* buf[BACKTRACE_STACK_MAX_SIZE];
      int nptr = backtrace(buf, BACKTRACE_STACK_MAX_SIZE);
      LOG(WARNING) << "backtrace() captured " << nptr << " addresses";
      char** strings = backtrace_symbols(buf, nptr);
      if (nullptr == strings) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
      }
      for (int i = 0; i < nptr; i++) {
        LOG(ERROR) << '[' << std::setw(2) << std::setfill('0') << i << "] "
                   << strings[i];
      }
      free(strings);
    }();
    LOG(WARNING) << "dump stack end...";
    signal(signo, SIG_DFL);
    raise(signo);
  }

 public:
  static Backtrace& GetBacktraceInstance() {
    static Backtrace ins;
    return ins;
  }
};

auto& INSTANCE_OF_BACKTRACE = Backtrace::GetBacktraceInstance();

}  // namespace utils