cmake_minimum_required(VERSION 3.27)
project(xoooooo)

set(CMAKE_CXX_STANDARD 20)

include_directories(inc)

# Automatically find all .cpp files in src/
file(GLOB SOURCES "src/*.cpp")

add_executable(xoooooo ${SOURCES})