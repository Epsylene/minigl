cmake_minimum_required(VERSION 3.10)
project(texture VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(${PROJECT_NAME} PUBLIC ${MGL_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE minigl)

# Copy resources
add_custom_target(copy_resources_${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/res
    ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMENT "Copying resource directory" VERBATIM
)

add_dependencies(${PROJECT_NAME} copy_resources_${PROJECT_NAME})