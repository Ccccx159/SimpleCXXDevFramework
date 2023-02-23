#include <map>

#include <sys/stat.h>

#include "nlohmann/json.hpp"
#include "myutils.hpp"

using json = nlohmann::json;

int Test_WriteJsonFile();
int Test_ReadJsonFile();
int Test_WriteTextFile();
int Test_ReadTextFile();
int Test_CreateEmptyFile();
int Test_IsFileExisted();
int Test_SearchFileRecursively();
int Test_GetFileNameWithoutSuffix();
int Test_GetSuffixFromFile();
int Test_ListFilesInDir();
int Test_MkdirsByPath();

int main(int argc, char* argv[]) {
  Logger_Init(argv[0]);
  Test_WriteJsonFile();
  Test_ReadJsonFile();
  Test_WriteTextFile();
  Test_ReadTextFile();
  Test_GetFileNameWithoutSuffix();
  Test_GetSuffixFromFile();
  Test_ListFilesInDir();
  Test_MkdirsByPath();
  Test_CreateEmptyFile();
  Test_IsFileExisted();
  Test_SearchFileRecursively();
  return 0;
}

int Test_WriteJsonFile() {
  json j = {
      {"name", "John"},
      {"native",
       {{"country", "China"}, {"province", "Zhejiang"}, {"city", "Hangzhou"}}},
      {"sex", "male"},
      {"age", 18}};
  CHECK_GT(myutils::WriteToFile("./.TempJsonFile.json", j), 0)
      << "Test Write Json File Failed!";
  LOG(INFO) << "Test Write Json File Successfully!";
  return 0;
}

int Test_ReadJsonFile() {
  json j;
  CHECK_GT(myutils::ReadFromFile("./.TempJsonFile.json", j), 0)
      << "Test Read Json File Failed!";
  LOG(INFO) << "Test Read Json File Successfully!";
  return 0;
}

int Test_WriteTextFile() {
  CHECK_GT(myutils::WriteToFile("./.TempTextFile.txt", "This is a test string"),
           0)
      << "Test Write Text File Failed!";
  LOG(INFO) << "Test Write Text File Successfully!";
  return 0;
}

int Test_ReadTextFile() {
  std::string s;
  CHECK_GT(myutils::ReadFromFile("./.TempTextFile.txt", s), 0)
      << "Test Read Text File Failed!";
  LOG(INFO) << "Test Read Text File Successfully!";
  return 0;
}

int Test_MkdirsByPath() {
  const static std::string path("./.Temp/mkdir_test_dir");
  CHECK_EQ(0, myutils::MkdirsByPath(path)) << "Test Mkdirs By Path failed!";
  struct stat statbuf;
  CHECK_EQ(0, stat(path.c_str(), &statbuf)) << "Test Mkdirs By Path failed!";
  LOG(INFO) << "Test Mkdirs By Path Successfully";
  return 0;
}

int Test_CreateEmptyFile() {
  const static std::string fp("./.Temp/test_create_file.txt");
  CHECK_EQ(0, myutils::CreateEmptyFile(fp)) << "Test Create Empty File Failed!";
  struct stat statbuf;
  CHECK_EQ(0, stat(fp.c_str(), &statbuf)) << "Test Create Empty File Failed!";
  LOG(INFO) << "Test Create Empty File Successfully";
  return 0;
}

int Test_IsFileExisted() {
  const static std::string fp("./.Temp/test_create_file.txt");
  struct stat statbuf;
  CHECK_EQ(stat(fp.c_str(), &statbuf), static_cast<int>(!myutils::IsFileExisted(fp))) << "Test Is File Existed Failed!";
  LOG(INFO) << "Test Is File Existed Successfully!";
  return 0;
}

int Test_SearchFileRecursively() {
  const static std::string f_keyword("test_create_file");
  const static std::string f_regex("test.+");
  std::string fullpath;
  struct stat statbuf;
  CHECK_EQ(true, myutils::SearchFileRecursively(fullpath, f_keyword, "./", 3)) << "Test Search File Recursively By Keywords Failed!";
  CHECK_EQ(0, stat(fullpath.c_str(), &statbuf)) << "Test Search File Recursively By Keywords Failed!";
  LOG(INFO) << "Test Search File Recursively By Keywords Successfully!";

  CHECK_EQ(true, myutils::SearchFileRecursively(fullpath, f_regex, "./", 3, "regex")) << "Test Search File Recursively By Regex Failed!";
  CHECK_EQ(0, stat(fullpath.c_str(), &statbuf)) << "Test Search File Recursively By Regex Failed!";
  LOG(INFO) << "Test Search File Recursively By Regex Successfully!";
  return 0;
}

int Test_GetFileNameWithoutSuffix() {
  const static std::map<std::string, std::string> TEST_DATA_GETFILENAME = {
      {"./test.cc", "test"},
      {"../CMakeLists.txt", "CMakeLists"},
      {"utils_file_unittest", "utils_file_unittest"},
      {"/usr/lib/back_up.tar.gz", "back_up"},
      {"./.TempTextFile.txt", ".TempTextFile"}};

  std::for_each(TEST_DATA_GETFILENAME.begin(), TEST_DATA_GETFILENAME.end(),
                [](const std::pair<std::string, std::string>& it) {
                  CHECK_EQ(myutils::GetFileNameWithoutSuffix(it.first), it.second)
                      << it.first << " get file name failed!";
                });
  LOG(INFO) << "Test GetFileNameWithoutSuffix successfully";
  return 0;
}

int Test_GetSuffixFromFile() {
  const static std::map<std::string, std::string> TEST_DATA_GETSUFFIX = {
      {"./test.cc", "cc"},
      {"../CMakeLists.txt", "txt"},
      {"utils_file_unittest", ""},
      {"/usr/lib/back_up.tar.gz", "tar.gz"},
      {"./.TempTextFile.txt", "txt"}};

  std::for_each(TEST_DATA_GETSUFFIX.begin(), TEST_DATA_GETSUFFIX.end(),
                [](const std::pair<std::string, std::string>& it) {
                  CHECK_EQ(myutils::GetSuffixFromFile(it.first), it.second)
                      << it.first << " get file name failed!";
                });
  LOG(INFO) << "Test GetSuffixFromFile successfully";
  return 0;
}

int Test_ListFilesInDir() {
  std::vector<std::string> fl = myutils::ListFilesInDir("./");
  CHECK_GT(fl.size(), 0) << "Test ListFilesInDir Failed!";
  LOG(INFO) << "Test ListFilesInDir successfully";
  return 0;
}