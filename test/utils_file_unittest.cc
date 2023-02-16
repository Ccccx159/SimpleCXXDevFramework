#include "utils/_file.hpp"
#include "logger/logger.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int Test_WriteJsonFile();
int Test_ReadJsonFile();
int Test_WriteTextFile();
int Test_ReadTextFile();


int main(int argc, char* argv[]) {
  Logger_Init(argv[0]);
  Test_WriteJsonFile();
  Test_ReadJsonFile();
  Test_WriteTextFile();
  Test_ReadTextFile();
  return 0;
}

int Test_WriteJsonFile() {
  json j = {
    {"name", "John"},
    {"native",{
      {"country","China"},
      {"province", "Zhejiang"},
      {"city", "Hangzhou"}
    }},
    {"sex", "male"},
    {"age", 18}
  };

  int wz = utils::WriteToFile("./.TempJsonFile.json", j);
  if (wz <= 0) {
    LOG(ERROR) << "TEst wRiTE JsON FilE failed!!!";
    return -1;
  } else {
    LOG(INFO) << "TEst wRiTE JsON FilE SucCEsSfULly";
    return wz;
  }
}

int Test_ReadJsonFile() {
  json j;
  int rz = utils::ReadFromFile("./.TempJsonFile.json", j);
  if (rz <= 0) {
    LOG(ERROR) << "TeSt rEAd JsON FilE failed!!!";
    return -1;
  }
  LOG(INFO) << "TeSt rEAd JsON FilE SucCEsSfULly";
  LOG(INFO) << '\n' << std::setw(4) << j;
  return rz;
}

int Test_WriteTextFile() {
  int wz = utils::WriteToFile("./.TempTextFile.txt", "This is a test string");
  if (wz <= 0) {
    LOG(ERROR) << "TEst wRiTE TeXT FilE failed!!!";
    return -1;
  }
  LOG(INFO) << "TEst wRiTE JsON FilE SUcCEssFUlLy";
  return wz;
}

int Test_ReadTextFile() {
  std::string s;
  int rz = utils::ReadFromFile("./.TempTextFile.txt", s);
  if (rz <= 0) {
    LOG(ERROR) << "TEst REaD TeXT FilE failed!!!";
    return -1;
  }
  LOG(INFO) << "TEst REaD TeXT FilE SUcCEssFUlLy!!!";
  LOG(INFO) << s;
  return rz;
}