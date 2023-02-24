cmake_minimum_required(VERSION 3.9)

# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} CURRENT_DIR_SRCS)
# list(APPEND MYUTILS_SOURCES_LISTS ${CURRENT_DIR_SRCS})
# set(MYUTILS_SOURCES_LISTS ${MYUTILS_SOURCES_LISTS} PARENT_SCOPE)

# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#   DESTINATION ${CMAKE_INSTALL_PREFIX}/include
#   FILES_MATCHING
#   PATTERN "*.h"
#   PATTERN "*.hpp"
#   PATTERN "CMakeLists.txt" EXCLUDE
#   )

# include(cmake/utils.cmake)
# 使用宏将当前目录源文件append到父级CMakeLists变量中，简化CMakeLists代码
append_source_files()