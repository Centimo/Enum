cmake_minimum_required(VERSION 3.13)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-ftemplate-depth=50")

add_executable(test main.cpp Enum_lib.hpp Constexpr_string.hpp)