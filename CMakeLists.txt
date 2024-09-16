cmake_minimum_required(VERSION 3.10)
project(glose)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Sources
set(SOURCES
    src/glose.hpp
    src/glose/hello_world.hpp
)

add_library(glose STATIC ${SOURCES})
set_target_properties(glose PROPERTIES LINKER_LANGUAGE CXX)

set(GLOSE_SOURCES src)
target_include_directories(glose PUBLIC ${GLOSE_SOURCES})

# Examples
add_subdirectory(examples/min_app)