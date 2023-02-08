cmake_minimum_required(VERSION 3.9)

# compiler configuration
if(CMAKE_VERSION VERSION_LESS "3.1")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GUN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
else()
  set(CMAKE_CXX_STANDARD 11)
endif()

include_directories(
  ./
  # ${GLOG_INCLUDE_DIR}
)

link_directories(
  # ${GLOG_LIBRARY_DIR}
)

add_subdirectory(common)
add_subdirectory(logger)
add_subdirectory(thread)
add_subdirectory(utils)

set(SRC_LIST ./main/main.cc)
message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
add_executable(HelloApp ${SRC_LIST})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(HelloApp module_logger module_thread module_utils)

INSTALL(TARGETS HelloApp
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
