#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "logger/logger.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace utils {

// 检查文件是否存在
bool IsFileExit(const std::string& f);

// 创建一个空文件
int CreateEmptyFile(const std::string& f);

// 在指定路径下递归查找特定文件
bool SearchFileFromPath(const std::string& f, const std::string& d);

// 递归查找文件
bool SearchFileRecursively(const std::string& f, const std::string& d, std::string& fp);

// 读取文本内容
int ReadFromFile(const std::string& f, json& buf,
                 std::ios::openmode m = std::ios::in);

int ReadFromFile(const std::string& f, std::string& buf,
                 std::ios::openmode m = std::ios::in);

// 截取不带扩展名的文件名
std::string& GetFileNameWithoutSuffix(const std::string& f,
                                      const std::string& sf);

// 截取文件扩展名
std::string& GetSuffixFromFile(const std::string& f);

// 列举指定目录下所有文件，
std::vector<std::string>& ListFilesInDir(const std::string& dir);

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

}  // namespace utils
