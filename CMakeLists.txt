set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

cmake_minimum_required(VERSION 3.20)

project(quartz)

set(CMAKE_CXX_STANDARD 20)

add_executable(qz src/main.cpp)