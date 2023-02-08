IF(CMAKE_VERSION VERSION_LESS "3.1")
    IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    ENDIF()
ELSE()
    SET(CMAKE_CXX_STANDARD 11)
ENDIF()
