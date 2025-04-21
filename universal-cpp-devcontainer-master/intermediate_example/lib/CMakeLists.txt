cmake_minimum_required( VERSION 3.23.0 )

project( intermediate_example_lib VERSION 1.0.0 )

add_library( intermediate_lib INTERFACE )

target_include_directories(
  intermediate_lib

  INTERFACE
    include/
)
