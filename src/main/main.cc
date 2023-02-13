#include <fstream>
#include <iostream>

#include "logger/logger.hpp"
#include "utils/utils.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) {
  Logger_Init(argv[0]);
  int verbose_flag = 0;
  // std::string sopt("ha:b:c:");
  struct option longOption[] = {/* These options set a flag. */
                                {"verbose", no_argument, &verbose_flag, 1},
                                {"brief", no_argument, &verbose_flag, 0},
                                /* These options don’t set a flag.
                                   We distinguish them by their indices. */
                                {"add", no_argument, 0, 'a'},
                                {"append", no_argument, 0, 'b'},
                                {"delete", required_argument, 0, 'd'},
                                {"create", required_argument, 0, 'c'},
                                {"file", required_argument, 0, 'f'},
                                {0, 0, 0, 0}};
  utils::Param::fn_paramParse f = [longOption](const int& c, const int& idx,
                                               const char* arg) {
    // LOG(WARNING) << "index: " << idx;
    switch (c) {
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (longOption[idx].flag != 0) break;
        // printf("option %s", longOption[idx].name);
        if (arg) // printf(" with arg %s", arg);
        // printf("\n");
        break;

      case 'a':
        // printf("option -a\n");
        break;

      case 'b':
        // printf("option -b\n");
        break;

      case 'c':
        // printf("option -c with value `%s'\n", arg);
        break;

      case 'd':
        // printf("option -d with value `%s'\n", arg);
        break;

      case 'f':
        // printf("option -f with value `%s'\n", arg);
        break;

      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        abort();
    }
  };
  utils::Param p(argc, argv, "abc:d:f:", longOption, f);
  p.PrintParams();
  // LOG(FATAL) << "this is a FATAL log by Google LOG";
  return 0;
}