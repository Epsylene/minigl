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
            GLFWwindow* get_native_window() { return _window; }
            
            ~Window() {
                glfwDestroyWindow(_window);
                glfwTerminate();
            }
            
            void set_viewport(const int width, const int height) {
                this->width = width;
                this->height = height;
                glViewport(0, 0, width, height);
            }

        public:

            unsigned int width, height;
            bool minimized;

        private:

            GLFWwindow* _window;
    };
}