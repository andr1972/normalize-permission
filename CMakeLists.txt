cmake_minimum_required(VERSION 3.21)
project(normalizeAttr)

set(CMAKE_CXX_STANDARD 17)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror=return-type")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(normalizeAttr main.cpp)
