cmake_minimum_required(VERSION 3.9)

include(cmake/utils.cmake)
hello_app_extract_version()

project(HelloApp VERSION ${HELLO_APP_VERSION} LANGUAGES CXX)

option(BUILD_TESTS "Build with tests" OFF)
option(ASAN_MEM_CHECK "Check memory with asan" OFF)

message(INFO "--------------------------------------")
message(STATUS "Build HelloApp: " ${HELLO_APP_VERSION})
message(STATUS "Build with Tests: " ${BUILD_TESTS})
message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
if (NOT CMAKE_TOOLCHAIN_FILE)
  message(STATUS "CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})
endif()

if (ASAN_MEM_CHECK)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_compile_options(-O0)
endif()

message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

message(INFO "--------------------------------------")
include(cmake/toolchain_options.cmake)
# include(cmake/load_3rdparty.cmake)

if(NOT APP_TARGET_OS)
  set(APP_TARGET_OS linux)
endif()
if(NOT APP_TARGET_ARCH)
  set(APP_TARGET_ARCH x86-64)
endif()
message(STATUS "HelloApp target os: " ${APP_TARGET_OS})
message(STATUS "HelloApp target arch: " ${APP_TARGET_ARCH})

# load_libglog(APP_TARGET_OS APP_TARGET_ARCH)

message(INFO "--------------------------------------")
add_subdirectory(3rdparty)

message(INFO "--------------------------------------")
add_subdirectory(src)

if(BUILD_TESTS)
  message(INFO "--------------------------------------")
  message(INFO "Build Unittest ON")
  add_subdirectory(test)
endif()




# SET_TARGET_PROPERTIES(MyLib PROPERTIES VERSION ${HELLO_APP_VERSION}
#                                           SOVERSION ${HELLO_APP_VERSION_MAJOR})
