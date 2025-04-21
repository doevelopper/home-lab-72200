cmake_minimum_required( VERSION 3.23.0 )

project( intermediate_example VERSION 1.0.0 )

add_executable( intermediate )

target_sources(
  intermediate

  PRIVATE FILE_SET HEADERS
    BASE_DIRS
      src/   # private local headers here
    FILES
      src/array_to_string.hpp
      src/trackers.hpp

  PRIVATE
    src/main.cpp
    src/trackers.cpp
)

target_link_libraries(
  intermediate

  PRIVATE
    intermediate_lib
)
