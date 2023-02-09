#include <fstream>
#include <iostream>

#include "logger/logger.hpp"
#include "utils/utils.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) {
  Logger_Init(argv[0]);
  LOG(INFO) << "FLAGS_logtostderr" << FLAGS_logtostderr << std::endl;
  LOG(INFO) << "FLAGS_stderrthreshold" << FLAGS_stderrthreshold << std::endl;
  LOG(INFO) << "FLAGS_minloglevel" << FLAGS_minloglevel << std::endl;
  LOG(INFO) << "this is a INFO log by Google LOG";
  LOG(WARNING) << "this is a WARNING log by Google LOG";
  LOG(ERROR) << "this is a ERROR log by Google LOG";
  // LOG(FATAL) << "this is a FATAL log by Google LOG";
  json j;
  std::ifstream _f("./test.json");
  if (!_f.is_open()) {
    LOG(ERROR) << "test.json opened failed...";
    exit(-1);
  }
  _f >> j;
  _f.close();
  return 0;
}