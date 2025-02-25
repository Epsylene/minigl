#pragma once

#include "mglpch.hpp"
#include "minigl/util/geometry.hpp"

#include "key_codes.h"
#include "mouse_codes.h"

using InputCode = int;

namespace mgl
{
    class Input
    {
        public:

            bool isKeyPressed(InputCode key)
            {
                return active[key];
            }

            bool isMouseButtonPressed(InputCode button)
            {
                return active[button];
            }

            bool cursorMoved()
            {
                return (currentMousePos != lastMousePos);
            }

            Vec2 getMousePos()
            {
                return currentMousePos;
            }

            Vec2 getMouseDelta()
            {
                return {
                    currentMousePos.x - lastMousePos.x,
                    lastMousePos.y - currentMousePos.y
                };
            }

        public:

            std::unordered_map<InputCode, bool> active;
            
            Vec2 currentMousePos;
            Vec2 lastMousePos;
    };
}