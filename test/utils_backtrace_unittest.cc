#include "utils/_backtrace.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[]) {
  Logger_Init(argv[0]);
  int *p = nullptr;
  *p = 5;
  return 0;
}