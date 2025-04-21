cmake_minimum_required(VERSION 3.23.0)

project(ucc_root VERSION 1.0.0)

# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
# endif()

# add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=1)
# set(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=1")
# add_compile_options(-fPIC)

add_subdirectory("exe")
add_subdirectory("lib")
