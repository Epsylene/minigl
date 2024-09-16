cmake_minimum_required(VERSION 3.10)

# Set the project name
project(min_app VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(min_app src/main.cpp)

include_directories(min_app PUBLIC ${GLOSE_SOURCES})
target_link_libraries(min_app glose)