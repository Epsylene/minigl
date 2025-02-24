#pragma once

#include "minigl/geometry.hpp"
#include <GLFW/glfw3.h>

namespace mgl
{
    /// Input class
    /// 
    /// This class handles all key and mouse inputs; however,
    /// it is not an event system class. Rather, it provides
    /// functions to retrieve information about the input
    /// event.
    class Input
    {
        public:

            /// Input data comes from GLFW, and as such
            /// is tied to a given GLFW window.
            Input(GLFWwindow* window):
                window(window) {}

            /// Is the key pressed ?
            bool isKeyPressed(int keyCode) const;

            /// Is the mouse button pressed ?
            bool isMouseButtonPressed(int button) const;

            /// Get the mouse X position
            float getMouseX() const;

            /// Get the mouse Y position
            float getMouseY() const;

            /// Get the mouse position, in window coordinates
            Vec2 getMousePos() const;

            /// Get the mouse position, in screen coordinates
            Vec2 getMouseScreenPos() const;

            /// Check whether the mouse is in a certain area:
            /// the area is defined by the rectangle that has
            /// `p0` and `p1` as bottom-left and upper-right
            /// vertices, respectively.
            ///
            /// @param screenCoords Set to true if `p0` and `p1` are
            ///     in screen coordinates
            bool mouseInArea(const Vec2& p0, const Vec2& p1, bool screenCoords = false) const;

        public:

            Vec2 lastMousePos;
        
        private:

            GLFWwindow* window;
    };
}