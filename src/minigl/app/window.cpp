#include "core.hpp"

#include "mglpch.hpp"
#include "window.hpp"

#include <glad/glad.h>

namespace mgl
{
    static bool glfwInitialized = false;

    static void glfwErrorCallback(int err, const char* description)
    {
        error("GLFW error ({}): {}", err, description);
    }

    Window::Window(std::string name, const int width, const int height) {
        // Init GLFW
        if (!glfwInitialized) {
            MGL_ASSERT(glfwInit(), "Failed to initialize GLFW");
            
            glfwSetErrorCallback(glfwErrorCallback);
            glfwInitialized = true;
        }

        // GLFW window
        _window = glfwCreateWindow((int)width, (int)height, name.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(_window);

        // Binds GLAD to GLFW's OpenGL context
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        MGL_ASSERT(status, "Failed to initialize GLAD.");

        // Print OpenGL info
        info("OpenGL info:");
        info("  Vendor: {}", (const char*)glGetString(GL_VENDOR));
        info("  Renderer: {}", (const char*)glGetString(GL_RENDERER));
        info("  Version: {}", (const char*)glGetString(GL_VERSION));
    }
}