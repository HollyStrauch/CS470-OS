cmake_minimum_required(VERSION 3.13)
project(cwuShell)

set(CMAKE_CXX_STANDARD 14)

add_executable(cwuShell main.cpp Shell.cpp Shell.h)