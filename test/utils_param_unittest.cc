#include "myutils.hpp"

int main(int argc, char* argv[]) {
  Logger_Init(argv[0]);

  int param_flag = 0;
  struct option lopt[] = {{"setflag", no_argument, &param_flag, 1},
                          {"unsetflag", no_argument, &param_flag, 0},
                          {"optionA", no_argument, NULL, 'a'},
                          {"optionB", optional_argument, NULL, 'b'},
                          {"optionC", required_argument, NULL, 'c'},
                          {0, 0, 0, 0}};

  myutils::Param::fn_paramParse f =
      [lopt, &param_flag](const int& c, const int& i, const char* a) {
        switch (c) {
          case 0:
            LOG(INFO) << "NOW, flag is [" << param_flag << ']';
            break;
          case 'a':
            break;
          case 'b':
            break;
          case 'c':
            break;
          case '?':
            break;
          default:
            LOG(FATAL) << "?? getopt returned character code 0x"
                       << std::setiosflags(std::ios::hex) << c;
        }
      };
  int caseNum = 5;
  char* _argv[][5] = {
      {"./utils_param_unittest"},
      {"./utils_param_unittest", "--setflag"},
      {"./utils_param_unittest", "--unsetflag"},
      {"./utils_param_unittest", "--optionA", "--optionB=paramB", "--optionC",
       "paramC"},
      {"./uitls_param_unittest", "-a", "-b", "paramB", "-cparamC"}};
  int _argc[] = {1, 2, 2, 5, 5};
  for (int i = 0; i < caseNum; i++) {
    std::string fullCmd;
    std::for_each(_argv[i], _argv[i] + _argc[i], [&fullCmd](const char* e) {
      fullCmd += e;
      fullCmd += ' ';
    });
    fullCmd[fullCmd.length() - 1] = '\0';
    LOG(INFO) << "full cmd is [" << fullCmd << ']';
    myutils::Param P(_argc[i], _argv[i], "ab:c:", lopt, f);
    P.PrintParams();
  }

  return 0;
}