cmake_minimum_required(VERSION 3.13)
project(xdmf)

set(CMAKE_CXX_STANDARD 11)

add_executable(xdmf XDMFFile.cpp XDMFFile.h pugixml.cpp main.cpp)