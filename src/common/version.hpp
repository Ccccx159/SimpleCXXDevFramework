#pragma once

#include <string>

// for cmake
// 用于在CMakeLists文件中解析用
// 0.1.0
#define HELLO_APP_VER_MAJOR 0
#define HELLO_APP_VER_MINOR 1
#define HELLO_APP_VER_PATCH 0

#define HELLO_APP_VERSION                                    \
  (HELLO_APP_VER_MAJOR * 10000 + HELLO_APP_VER_MINOR * 100 + \
   HELLO_APP_VER_PATCH)

// for source code
// 用于在项目源码中获取版本号字符串
// v0.1.0
#define _HELLO_APP_STR(s) #s
#define HELLO_PROJECT_VERSION(major, minor, patch) \
  "v" _HELLO_APP_STR(major.minor.patch)

namespace myutils {

inline std::string GetVersionStr(void) {
  return HELLO_PROJECT_VERSION(HELLO_APP_VER_MAJOR, HELLO_APP_VER_MINOR,
                               HELLO_APP_VER_PATCH);
}

}  // namespace myutils