#include "_param.hpp"

#include <algorithm>
#include <vector>

#include "logger/logger.hpp"

namespace utils {
Param::Param(const int& argc, char** argv, const char* sopt,
             const struct option lopt[], fn_paramParse& f) {
  int c = 0;
  while (1) {
    int optIdx = 0;
    c = getopt_long(argc, argv, sopt, lopt, &optIdx);

    if (-1 == c) {
      LOG(WARNING) << "no more params waiting to be parsed...";
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
  std::string key, val, tmpArg;
  size_t tmpPos = 0;
  for(auto it = argList.begin();it != argList.end(); it++) {
    tmpArg = *it;
    if (tmpArg[0] == '-' && tmpArg[1] == '-') {
      if ((tmpPos = tmpArg.find('=')) != tmpArg.npos) {
        key = tmpArg.substr(2, tmpPos - 2);
        val = tmpArg.substr(tmpPos + 1, tmpArg.length() + tmpPos);
      } else {
        key = tmpArg.substr(2, tmpArg.length() - 1);
        auto nextIt = it + 1;
        if ((*nextIt)[0] == '-') {
          val = "null";
        } else {
          val = *nextIt;
          it++;
        }
      }
    } else if (tmpArg[0] == '-' && tmpArg[1] != '-') {
      key = tmpArg[1];
      if (tmpArg.length() > 2) {
        val = tmpArg.substr(2, tmpArg.length() - 1);
      } else {
        auto nextIt = it + 1;
        if ((*nextIt)[0] == '-') {
          val = "null";
        } else {
          val = *nextIt;
          it++;
        }
      }
    } else {
      LOG(ERROR) << "argv: " << *it << "is wrong!";
    }
    DLOG(INFO) << "param: key: " << key << ", val: " << val;
    _params.emplace(std::make_pair(key, val));
  }
  return 0;
}

}  // namespace utils