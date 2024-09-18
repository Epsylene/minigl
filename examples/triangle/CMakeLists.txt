cmake_minimum_required(VERSION 3.10)

project(triangle VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(${PROJECT_NAME} PUBLIC ${MGL_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE minigl)

# Copy resources
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/res
    ${CMAKE_CURRENT_BINARY_DIR}/res
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res
    COMMENT "Copying resource directory" VERBATIM
)

# Set so that the shader file is copied each time its contents
# are changed (no configure_directory command, sadly...)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/res/flat.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/res/flat.glsl COPYONLY
)