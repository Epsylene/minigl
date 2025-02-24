#pragma once

#include "mglpch.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace mgl
{
    class Window 
    {
        public:

            Window(const int width, const int height);
            GLFWwindow* get_native_window() { return window; }
            
            ~Window() {
                glfwDestroyWindow(window);
                glfwTerminate();
            }
            
            void set_viewport(const int width, const int height) {
                this->width = width;
                this->height = height;
                glViewport(0, 0, width, height);
            }

        private:

            GLFWwindow* window;
            unsigned int width, height;
    };
}