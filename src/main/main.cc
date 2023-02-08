#include <iostream>
#include <fstream>

#include "logger/logger.hpp"
#include "utils/utils.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) { 
  std::cout << "FLAGS_logtostderr" << FLAGS_logtostderr << std::endl;
  std::cout << "FLAGS_stderrthreshold" << FLAGS_stderrthreshold  << std::endl;
  std::cout << "FLAGS_minloglevel" << FLAGS_minloglevel << std::endl;
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);
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