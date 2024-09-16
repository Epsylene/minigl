cmake_minimum_required(VERSION 3.10)
project(glose)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Sources
add_library(glose STATIC src/glose.hpp)
set_target_properties(glose PROPERTIES LINKER_LANGUAGE CXX)