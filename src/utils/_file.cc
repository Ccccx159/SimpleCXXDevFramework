#include "_file.hpp"

#include <dirent.h>

#include <regex>

namespace utils {

// 检查文件是否存在
bool IsFileExisted(const std::string& f) {
  struct stat buf;
  if (0 != stat(f.c_str(), &buf)) {
    LOG(WARNING) << "file [" << f << "] is NOT Existed";
    return false;
  }
  DLOG(INFO) << "file [" << f << "] is Existed";
  return true;
}

// 创建一个空文件
int CreateEmptyFile(const std::string& f) {
  std::ofstream _of(f, std::ios::out);
  if (!_of.is_open()) {
    LOG(ERROR) << "Create file[" << f << "] failed!";
    return -1;
  }
  DLOG(INFO) << "File created Successfully~ [" << f << ']';
  _of.close();
  return 0;
}

// 递归查找文件
bool SearchFileRecursively(std::string& fp, const std::string& f,
                           const std::string& d, int depth,
                           const std::string& pattern) {
  if (depth <= 0) return false;
  struct dirent* end = nullptr;
  std::string curDir = d[d.length() - 1] == '/' ? d : d + '/';
  std::string tmpFile;
  DIR* fDir = opendir(curDir.c_str());
  if (nullptr == fDir) {
    LOG(ERROR) << "Open directory[" << curDir << "] failed!";
    return false;
  }
  while ((end = readdir(fDir)) != nullptr) {
    if (std::string(end->d_name) == "." || std::string(end->d_name) == "..") {
      continue;
    }
    if (end->d_type == DT_DIR) {
      std::string nextDir = curDir + end->d_name;
      if (true == SearchFileRecursively(fp, f, nextDir, depth - 1, pattern)) {
        DLOG(INFO) << "Search file Successfully~ [" << fp << ']';
        closedir(fDir);
        return true;
      }
      continue;
    }
    tmpFile = end->d_name;
    if (pattern == "keyword") {
      if (tmpFile.npos != tmpFile.find(f)) {
        fp = curDir + tmpFile;
        DLOG(INFO) << "Search file Successfully~ [" << fp << ']';
        closedir(fDir);
        return true;
      }
    } else if (pattern == "regex") {
      std::regex e(f);
      if (std::regex_search(tmpFile, e)) {
        fp = curDir + tmpFile;
        DLOG(INFO) << "Search file Successfully~ [" << fp << ']';
        closedir(fDir);
        return true;
      }
    }
  }
  DLOG(ERROR) << "File [" << f << "] can't be found under the dir[" << d << ']';
  closedir(fDir);
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
std::string GetFileNameWithoutSuffix(const std::string& f) {
  std::string name;
  std::string head;
  size_t pos = f.rfind('/') == f.npos ? 0 : f.rfind('/') + 1;
  name = f.substr(pos, f.length() - pos);
  if ('.' == name[0]) {
    name = name.substr(1, name.length() - 1);
    head = ".";
  }
  pos = name.find('.') == f.npos ? name.length() : name.find('.');
  name = name.substr(0, pos);
  name = head + name;
  // LOG(INFO) << "File's  name is [" << name << ']';
  return name;
}

// 截取文件扩展名
std::string GetSuffixFromFile(const std::string& f) {
  std::string suffix;
  size_t pos = f.rfind('/') == f.npos ? 0 : f.rfind('/') + 1;
  suffix = f.substr(pos, f.length() - pos);
  // 当文件名以 '.' 开头时，排除该 '.'
  if ('.' == suffix[0]) suffix = suffix.substr(1, suffix.length() - 1);
  pos =
      suffix.find('.') == suffix.npos ? suffix.length() : suffix.find('.') + 1;
  suffix = suffix.substr(pos, suffix.length() - pos);
  return suffix;
}

// 列举指定目录下所有文件，
std::vector<std::string> ListFilesInDir(const std::string& dir) {
  std::vector<std::string> fl;
  DIR* fDir = opendir(dir.c_str());
  if (nullptr == fDir) {
    LOG(ERROR) << "Open directory[" << dir << "] failed!";
    return fl;
  }
  struct dirent* end = nullptr;
  while ((end = readdir(fDir)) != nullptr) {
    if (end->d_type != DT_DIR) {
      fl.push_back(dir[dir.length() - 1] == '/' ? dir + end->d_name
                                                : dir + "/" + end->d_name);
    }
  }
#ifdef NDEBUG
  std::for_each(fl.begin(), fl.end(),
                [](const std::string& it) { DLOG(INFO) << it; });
#endif
  closedir(fDir);
  return fl;
}

// 创建层级目录
int MkdirsByPath(const std::string& p, mode_t m) {
  if (access(p.c_str(), F_OK) == 0) {
    // LOG(WARNING) << p << " already exist!";
  } else {
    int ret = MkdirsByPath(p.substr(0, p.rfind('/')), m);
    if (0 == ret) {
      ret = mkdir(p.c_str(), m);
      LOG_IF(ERROR, 0 != ret) << "mkdir [" << p << "] Failed!";
    }
    return ret;
  }
  return 0;
}

}  // namespace utils