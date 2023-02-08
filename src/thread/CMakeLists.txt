cmake_minimum_required(VERSION 3.9)

aux_source_directory(. CURRENT_DIR_SRCS)
add_library(module_thread ${CURRENT_DIR_SRCS})
target_link_libraries(module_thread glog::glog nlohmann_json::nlohmann_json)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.hpp"
  PATTERN "CMakeLists.txt" EXCLUDE
  )
