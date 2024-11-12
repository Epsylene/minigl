cmake_minimum_required(VERSION 3.10)
project(minigl)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Sources
set(SOURCES
    src/core.hpp
    src/log.hpp
    src/minigl/minigl.hpp

    src/minigl/render_command.cpp
    src/minigl/render_command.hpp
    src/minigl/buffer.cpp
    src/minigl/buffer.hpp
    src/minigl/shader.cpp
    src/minigl/shader.hpp
    
    src/minigl/mesh.cpp
    src/minigl/mesh.hpp
    src/minigl/texture.cpp
    src/minigl/texture.hpp
    
    src/minigl/geometry.cpp
    src/minigl/geometry.hpp
    src/minigl/color.cpp
    src/minigl/color.hpp
    
    src/minigl/app/app.hpp
    src/minigl/app/app.cpp
    src/minigl/app/app_3d.hpp
    
    src/minigl/app/window.hpp
    src/minigl/app/window.cpp
    src/minigl/app/input/input.cpp
    src/minigl/app/input/input.hpp
    src/minigl/app/input/key_codes.h
    src/minigl/app/input/mouse_codes.h
    
    src/minigl/app/camera.cpp
    src/minigl/app/camera.hpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Include directories
set(MGL_INCLUDE src)
target_include_directories(${PROJECT_NAME} PUBLIC ${MGL_INCLUDE})

# Libraries
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm)
add_subdirectory(lib/tinyobj)
add_subdirectory(lib/fmt)
add_subdirectory(lib/stb_image)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw ${GLFW_LIBRARIES} opengl32 glad glm fmt stb_image tinyobjloader)
target_link_libraries(${PROJECT_NAME} PUBLIC -Wl,-static -static-libgcc -static-libstdc++ -lstdc++fs)

# Avoid clashes between GLFW and Glad
target_compile_definitions(${PROJECT_NAME} PUBLIC -DGLFW_INCLUDE_NONE)

# Pre-compiled headers
target_precompile_headers(${PROJECT_NAME} PUBLIC src/mglpch.hpp)

# Examples
add_subdirectory(examples/min_app)
add_subdirectory(examples/triangle)
add_subdirectory(examples/mesh)
add_subdirectory(examples/texture)
add_subdirectory(examples/shadows)
add_subdirectory(examples/instancing)