cmake_minimum_required(VERSION 3.9)

option(WITH_SHARED_LIB "Build shared libraries in the module" OFF)

# compiler configuration
if(CMAKE_VERSION VERSION_LESS "3.1")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GUN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  endif()
else()
  set(CMAKE_CXX_STANDARD 14)
endif()

include_directories(
  ./
  # ${GLOG_INCLUDE_DIR}
)

link_directories(
  # ${GLOG_LIBRARY_DIR}
)

if (WITH_SHARED_LIB)
  set(BUILD_LIBS_TYPE "SHARED")
else ()
  set(BUILD_LIBS_TYPE "STATIC")
endif()

add_subdirectory(common)
add_subdirectory(logger)
add_subdirectory(thread)
add_subdirectory(debug)
add_subdirectory(param)
add_subdirectory(system)
add_subdirectory(container)

aux_source_directory(. CURRENT_DIR_SRCS)
list(APPEND MYUTILS_SOURCES_LISTS ${CURRENT_DIR_SRCS})
# message(STATUS ${MYUTILS_SOURCES_LISTS})
# message(STATUS ${MODULE_LIBS_TYPE})
add_library(myutils ${BUILD_LIBS_TYPE} ${MYUTILS_SOURCES_LISTS})
target_link_libraries(myutils glog::glog nlohmann_json::nlohmann_json)

install(TARGETS myutils DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.hpp"
  PATTERN "CMakeLists.txt" EXCLUDE
  )