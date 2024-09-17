cmake_minimum_required(VERSION 3.10)
project(glose)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Sources
set(SOURCES
    src/core.hpp
    src/glose/glose.hpp

    src/glose/render_command.cpp
    src/glose/render_command.hpp
    src/glose/buffer.cpp
    src/glose/buffer.hpp
    src/glose/shader.cpp
    src/glose/shader.hpp

    src/glose/geometry.hpp
    src/glose/color.cpp
    src/glose/color.hpp

    src/glose/app/app.hpp
    src/glose/app/app.cpp
    src/glose/app/window.hpp
    src/glose/app/window.cpp
)

add_library(glose STATIC ${SOURCES})
set_target_properties(glose PROPERTIES LINKER_LANGUAGE CXX)

# Include directories
set(GLOSE_INCLUDE src)
target_include_directories(glose PUBLIC ${GLOSE_INCLUDE})

# Libraries
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glad EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(glose glfw ${GLFW_LIBRARIES} opengl32 glad glm)

# Avoid clashes between GLFW and Glad
add_definitions(-DGLFW_INCLUDE_NONE)

# Pre-compiled headers
target_precompile_headers(glose PUBLIC src/glspch.hpp)

# Examples
add_subdirectory(examples/min_app)
add_subdirectory(examples/triangle)