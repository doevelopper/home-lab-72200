cmake_minimum_required( VERSION 3.23.0 )

project( basic_example VERSION 1.0.0 )

add_executable( basic )

target_sources(
  basic

  PRIVATE FILE_SET HEADERS
    BASE_DIRS
      src/   # private local headers here
    FILES
      src/vector_to_string.hpp

  PRIVATE
    src/vector_to_string.cpp   # sources here
    src/main.cpp
)

target_include_directories(
  basic

  PUBLIC
    include/  # public headers here

  # PRIVATE
    # ${GTEST_INCLUDE_DIRS}   # private external headers here
)
