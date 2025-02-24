#pragma once

#include "core.hpp"
#include "mglpch.hpp"

#include "input/input.hpp"
#include "window.hpp"

namespace mgl
{
    class Event
    {
        public:

            Event(Ref<Window> window):
                window(window), quit(false)
            {
                input = ref<Input>();
            }

            void onWindowClose() {
                quit = true;
            }

            void onWindowResize(int new_width, int new_height) {
                window->set_viewport(new_width, new_height);
                window->minimized = (new_width == 0 || new_height == 0);
            }

            void onKeyEvent(int key, int scancode, int action, int mods) {
                // Register key press
                if (action == GLFW_PRESS || action == GLFW_REPEAT)
                    input->active[key] = true;
                else if (action == GLFW_RELEASE)
                    input->active[key] = false;

                // ESC: quit
                if (input->active[GLFW_KEY_ESCAPE])
                    quit = true;
            }

            void onMouseEvent(int button, int action, int mods) {
                // Register mouse button press
                if (action == GLFW_PRESS)
                    input->active[button] = true;
                else if (action == GLFW_RELEASE)
                    input->active[button] = false;
            }

            void onCursorEvent(double xpos, double ypos) {
                input->currentMousePos = {xpos, ypos};
            }

        public:

            Ref<Window> window;
            Ref<Input> input;
            bool quit;

            std::function<void()> handler;
    };
}