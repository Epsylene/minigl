#pragma once

#include "glspch.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace glose
{
    class Window 
    {
        public:

            Window(const int width, const int height);
            GLFWwindow* get_native_window() { return window; }
            
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