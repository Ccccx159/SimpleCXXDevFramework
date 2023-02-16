#include <fstream>
#include <iostream>

#include "logger/logger.hpp"
#include "utils/utils.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) {
  Logger_Init(argv[0]);
  LOG(INFO) << "Hello World !";
  return 0;
}