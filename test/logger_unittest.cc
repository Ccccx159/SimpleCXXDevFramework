#include "myutils.hpp"

int main(int argc, char* argv[]) {
  Logger_Init(argv[0]);
  LOG(INFO) << "This is a Google::INFO log";
  LOG(WARNING) << "This is a Google::WARNING log";
  LOG(ERROR) << "This is a Google::ERROR log";

  return 0;
}
