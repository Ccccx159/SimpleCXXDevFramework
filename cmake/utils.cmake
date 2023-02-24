function(hello_app_extract_version)
  file(READ "${CMAKE_CURRENT_LIST_DIR}/src/common/version.hpp" file_contents)
  string(REGEX MATCH "HELLO_APP_VER_MAJOR ([0-9]+)" _ "${file_contents}")
  if(NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(FATAL_ERROR "Could not extract major version number from version.hpp")
  endif()
  set(ver_major ${CMAKE_MATCH_1})
  
  string(REGEX MATCH "HELLO_APP_VER_MINOR ([0-9]+)" _ "${file_contents}")
  if (NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(FATAL_ERROR "Could not extract minor version number from version.hpp")
  endif()
  set(ver_minor ${CMAKE_MATCH_1})
  
  string(REGEX MATCH "HELLO_APP_VER_PATCH ([0-9]+)" _ "${file_contents}")
  if (NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(FATAL_ERROR "Could not extract patch version number from version.hpp")
  endif()
  set(ver_patch ${CMAKE_MATCH_1})

  set(HELLO_APP_VERSION_MAJOR ${ver_major} PARENT_SCOPE)
  set(HELLO_APP_VERSION "${ver_major}.${ver_minor}.${ver_patch}" PARENT_SCOPE)
endfunction()

macro(append_source_files)
  aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} CURRENT_DIR_SRCS)
  list(APPEND MYUTILS_SOURCES_LISTS ${CURRENT_DIR_SRCS})
  set(MYUTILS_SOURCES_LISTS ${MYUTILS_SOURCES_LISTS} PARENT_SCOPE)

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "CMakeLists.txt" EXCLUDE
    )
endmacro()