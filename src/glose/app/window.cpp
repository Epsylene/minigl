#include "core.hpp"

#include "glspch.hpp"
#include "window.hpp"

#include "glad/glad.h"

namespace glose
{
    static bool glfwInitialized = false;

    static void glfwErrorCallback(int error, const char* description)
    {
        printf("GLFW error (%i): %s", error, description);
    }

    Window::Window(const int width, const int height) {
        // Init GLFW
        if (!glfwInitialized) {
            GLS_ASSERT(glfwInit(), "Failed to initialize GLFW");
            
            glfwSetErrorCallback(glfwErrorCallback);
            glfwInitialized = true;
        }

        // GLFW window
        window = glfwCreateWindow((int)width, (int)height, "App window", nullptr, nullptr);
        glfwMakeContextCurrent(window);

        // Binds GLAD to GLFW's OpenGL context
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        GLS_ASSERT(status, "Failed to initialize GLAD.");

        // Print OpenGL info
        printf("OpenGL info:\n");
        printf("  Vendor: %s\n", glGetString(GL_VENDOR));
        printf("  Renderer: %s\n", glGetString(GL_RENDERER));
        printf("  Version: %s\n", glGetString(GL_VERSION));
    }
}