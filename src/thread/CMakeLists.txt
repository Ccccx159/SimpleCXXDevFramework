cmake_minimum_required(VERSION 3.9)

aux_source_directory(. CURRENT_DIR_SRCS)
add_library(module_thread ${MODULE_LIBS_TYPE} ${CURRENT_DIR_SRCS})
target_link_libraries(module_thread glog::glog nlohmann_json::nlohmann_json)

if (${MODULE_LIBS_TYPE} STREQUAL "SHARED")
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "CMakeLists.txt" EXCLUDE
    )

  install(TARGETS module_thread DESTINATION lib)
endif()