#pragma once

#include <sys/stat.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "logger/logger.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace myutils {

// 检查文件是否存在
bool IsFileExisted(const std::string& f);

// 创建一个空文件
int CreateEmptyFile(const std::string& f);

// 递归查找文件
bool SearchFileRecursively(std::string& fp, const std::string& f,
                           const std::string& d, int depth = 1,
                           const std::string& pattern = "keyword");

// 读取文本内容
int ReadFromFile(const std::string& f, json& buf,
                 std::ios::openmode m = std::ios::in);

int ReadFromFile(const std::string& f, std::string& buf,
                 std::ios::openmode m = std::ios::in);

// 截取不带扩展名的文件名
std::string GetFileNameWithoutSuffix(const std::string& f);

// 截取文件扩展名
std::string GetSuffixFromFile(const std::string& f);

// 列举指定目录下特定类型，或符合关键字的所有内容。type: dir/file
std::vector<std::string> ListFilesInDir(const std::string& dir,
                                        const std::string& type,
                                        const std::string& keyword = "");

// 创建层级目录
int MkdirsByPath(const std::string& p,
                 mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO);

// 写文件
template <typename T>
int WriteToFile(const std::string& f, const T& buf,
                std::ios::openmode m = std::ios::out) {
  std::ofstream _of(f);
  if (!_of.is_open()) {
    LOG(ERROR) << "File[" << f << "] open failed!!!";
    return -1;
  }
  _of << buf;
  _of.seekp(0, std::ios::end);
  std::streampos wz = _of.tellp();
  _of.close();
  return static_cast<long>(wz);
}

//

}  // namespace myutils
