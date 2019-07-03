cmake_minimum_required(VERSION 3.5)

project(xdmf)

include_directories(C:/cygwin64/usr/include/python3.6m)

find_package(pybind11 REQUIRED)
# adds a library pybindings that can be called by python
pybind11_add_module(pybindings pugixml.cpp XDMFFile.cpp main.cpp)

message("${pybind11_VERSION}")
message("${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
# -------------------------------------------------------
add_executable(xdmf pugixml.cpp XDMFFile.cpp main.cpp)