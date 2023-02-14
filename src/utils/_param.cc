#include "_param.hpp"

#include <algorithm>
#include <vector>

#include "logger/logger.hpp"

namespace utils {
Param::Param(const int& argc, char** argv, const char* sopt,
             const struct option lopt[], fn_paramParse& f) {
  if (argc <= 1) {
    LOG(WARNING) << "no params waiting to be parsed...";
  }
  int c = 0;
  while (1) {
    int optIdx = 0;
    c = getopt_long(argc, argv, sopt, lopt, &optIdx);

    if (-1 == c) {
      DLOG(WARNING) << "no more params waiting to be parsed...";
      break;
    }

    f(c, optIdx, optarg);
  }
  ParseParams(argc, (const char**)argv);
}

int Param::QueryParams(const std::string& opt,
                       std::pair<std::string, std::string>& param) {
  auto it = _params.find(opt);
  if (it == _params.end()) {
    LOG(ERROR) << "Cannot find option (" << opt << ")";
    return -1;
  }
  param.first = it->first;
  param.second = it->second;
  return 0;
}

void Param::PrintParams(void) {
  std::for_each(_params.begin(), _params.end(), [](auto& p) {
    LOG(INFO) << "opt: -" << p.first << ' ' << "arg:  " << p.second;
  });
}

int Param::ParseParams(const int& argc, const char** argv) {
  std::string origParam;
  std::for_each(argv + 1, argv + argc, [&origParam](auto& a) {
    origParam += a;
    origParam += ' ';
  });
  origParam = origParam.substr(0, origParam.length());
  DLOG(INFO) << "origin argv string is: [" << origParam << ']';

  std::vector<std::string> argList;
  for (int prePos = 0, curPos = 0; curPos < origParam.length(); curPos++) {
    if (origParam[curPos] != ' ') {
      continue;
    } else {
      argList.push_back(origParam.substr(prePos, curPos - prePos));
      prePos = curPos + 1;
    }
  }
  std::for_each(argList.begin(), argList.end(),
                [](auto e) { DLOG(INFO) << e; });
  std::string key, val;
  size_t tmpPos = 0;
  for (auto pos = 0; pos < argList.size(); pos++) {
    auto& a = argList[pos];
    if (a.length() < 2) {
      LOG(ERROR) << "?? Wrong option with " << a << "??";
      break;
    }
    if (a[0] == '-' && a[1] == '-') {
      if ((tmpPos = a.find('=')) != a.npos) {
        key = a.substr(2, tmpPos - 2);
        val = a.substr(tmpPos + 1, a.length() - tmpPos);
      } else {
        key = a.substr(2, a.length() - 1);
        if (pos + 1 < argList.size()) {
          auto& n_a = argList[pos + 1];
          if (n_a[0] == '-') {
            val = "null";
          } else {
            val = n_a;
            pos++;
          }
        } else {
          val = "null";
        }
      }
    } else if (a[0] == '-' && a[1] != '-') {
      key = a[1];
      if (a.length() > 2) {
        val = a.substr(2, a.length() - 1);
      } else {
        if (pos + 1 < argList.size()) {
          auto& n_a = argList[pos + 1];
          if (n_a[0] == '-') {
            val = "null";
          } else {
            val = n_a;
            pos++;
          }
        }
      }
    } else {
      LOG(ERROR) << "?? Wrong Option with " << a << "??";
      break;
    }
    DLOG(INFO) << "Option: " << key << ", Param: " << val;
    _params.emplace(std::make_pair(key, val));
  }
  return 0;
}

}  // namespace utils