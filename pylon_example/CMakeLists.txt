cmake_minimum_required(VERSION 3.24)
project(pylon_example)

set(CMAKE_CXX_STANDARD 17)
find_package(pylon 7.2.1 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_executable(pylon_example main.cpp Grabber.h)
target_link_libraries( pylon_example PRIVATE pylon::pylon pthread)