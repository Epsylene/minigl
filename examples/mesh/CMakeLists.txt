cmake_minimum_required(VERSION 3.10)

project(mesh VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(${PROJECT_NAME} PUBLIC ${GLOSE_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE glose)

# Copy resources
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res
    ${CMAKE_CURRENT_BINARY_DIR}/res
)