#pragma once

#include <getopt.h>

#include <functional>
#include <string>
#include <unordered_map>

namespace myutils {

class Param {
 public:
  Param() = delete;
  Param(const Param&) = delete;
  Param& operator=(const Param&) = delete;

  using fn_paramParse =
      std::function<void(const int&, const int&, const char*)>;
  Param(const int& argc, char** argv, const char* sopt,
        const struct option lopt[], fn_paramParse& f);

  int QueryParams(const std::string& opt,
                  std::pair<std::string, std::string>& param);

  void PrintParams(void);

 private:
  int ParseParams(const int& argc, const char** argv);

 private:
  std::unordered_map<std::string, std::string> _params;
};

}  // namespace myutils