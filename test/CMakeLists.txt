cmake_minimum_required(VERSION 3.9)
PROJECT(unittest)

include_directories(
  ${CMAKE_SOURCE_DIR}/src
)
aux_source_directory(. unittest_sources_list)
message(STATUS ${unittest_sources_list})
foreach(source_file ${unittest_sources_list})
  # 匹配 *.c *.cc *.cpp *.cxx
  STRING(REGEX REPLACE ".+/(.*)\\.c.+" "\\1" unittest_name ${source_file})
  add_executable(${unittest_name} ${source_file})
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/unittest)
  target_link_libraries(${unittest_name} module_logger module_utils)
  install(TARGETS ${unittest_name}
          RUNTIME DESTINATION unittest
          )
endforeach()