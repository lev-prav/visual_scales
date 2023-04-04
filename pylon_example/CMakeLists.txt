cmake_minimum_required(VERSION 3.24)
project(two_baslers)

set(CMAKE_CXX_STANDARD 17)
find_package(pylon 7.2.1 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_library(two_baslers main.cpp Grabber.h)
target_link_libraries( two_baslers PRIVATE pylon::pylon pthread)