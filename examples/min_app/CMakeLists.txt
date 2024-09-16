cmake_minimum_required(VERSION 3.10)

project(min_app VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(${PROJECT_NAME} PUBLIC ${GLOSE_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE glose)