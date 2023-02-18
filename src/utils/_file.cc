#include "_file.hpp"

#include <dirent.h>
#include <sys/stat.h>

#include <regex>

namespace utils {

// 检查文件是否存在
bool IsFileExisted(const std::string& f) {
  struct stat buf;
  if (0 != stat(f.c_str(), &buf)) {
    LOG(WARNING) << "file [" << f << "] is NOT Existed O_o";
    return false;
  }
  LOG(INFO) << "file [" << f << "] is Existed ^_^";
  return true;
}

// 创建一个空文件
int CreateEmptyFile(const std::string& f) {
  std::ofstream _of(f, std::ios::out);
  if (!_of.is_open()) {
    LOG(ERROR) << "Create file[" << f << "] failed!";
    return -1;
  }
  LOG(INFO) << "File created Successfully~ [" << f << ']';
  _of.close();
  return 0;
}

// 递归查找文件
bool SearchFileRecursively(std::string& fp, const std::string& f,
                           const std::string& d, int depth,
                           const std::string& pattern) {
  if (depth <= 0) return false;
  struct dirent* end = nullptr;
  std::string curDir(d + '/');
  std::string tmpFile;
  Dir* fDir = opendir(curDir.c_str());
  if (nullptr == fDir) {
    LOG(ERROR) << "Open directory[" << curDir << "] failed!";
    return false;
  }
  while (end = readdir(fDir) != nullptr) {
    if (std::string(end->d_name) == '.' || std::string(end->d_name) == "..") {
      continue;
    }
    if (end->d_type == DT_DIR) {
      return SearchFileRecursively(f, curDir + end.d_name, fp, depth - 1);
    }
    tmpFile = end->d_name;
    if (pattern == "keyword") {
      if (tmpFile.npos != tmpFile.find(f)) {
        fp = curDir + tmpFile;
        LOG(INFO) << "Search file Successfully~ [" << fp << ']';
        return true;
      }
    } else if (pattern == "regex") {
      std::regex e(f);
      if (std::regex_search(tmpFile, f)) {
        fp = curDir + tmpFile;
        LOG(INFO) << "Search file Successfully~ [" << fp << ']';
        return true;
      }
    }
  }
  LOG(ERROR) << "File [" << f << "] can't be found under the dir[" << d << ']';
  return false;
}

// 读取文本内容
int ReadFromFile(const std::string& f, json& buf, std::ios::openmode m) {
  std::ifstream _if(f, m);
  if (!_if.is_open()) {
    LOG(ERROR) << "File [" << f << "] open failed!!!";
    return -1;
  }
  _if >> buf;
  _if.seekg(0, std::ios::end);
  std::streampos rz = _if.tellg();
  _if.close();
  return static_cast<long>(rz);
}

int ReadFromFile(const std::string& f, std::string& buf, std::ios::openmode m) {
  std::ifstream _if(f, m);
  if (!_if.is_open()) {
    LOG(ERROR) << "File [" << f << "] open failed!!!";
    return -1;
  }
  char _cache[1024] = {0};
  while (_if.getline(_cache, sizeof(_cache))) {
    buf = buf + std::string(_cache) + '\n';
  }
  _if.close();
  return buf.length();
}

// 截取不带扩展名的文件名
std::string GetFileNameWithoutSuffix(const std::string& f,
                                     const std::string& sf) {
  std::string fn;
  return fn;
}

// 截取文件扩展名
std::string GetSuffixFromFile(const std::string& f) {
  std::string sf;
  return sf;
}

// 列举指定目录下所有文件，
std::vector<std::string> ListFilesInDir(const std::string& dir) {
  std::vector<std::string> fl;
  return fl;
}

}  // namespace utils