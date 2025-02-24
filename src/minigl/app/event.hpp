#pragma once

#include "core.hpp"
#include "mglpch.hpp"

#include "input/input.hpp"
#include "window.hpp"

namespace mgl
{
    using EventHandler = std::function<void(Ref<Input>, float)>;

    class Event
    {
        public:

            Event(Ref<Window> window, Ref<Input> input):
                window(window), input(input), quit(false)
            {}

            void onWindowClose() {
                quit = true;
            }

            void onWindowResize(int new_width, int new_height) {
                window->set_viewport(new_width, new_height);
                window->minimized = (new_width == 0 || new_height == 0);
            }

            void onKeyEvent(int key, int action, float dt) {
                // Register key press
                if (action == GLFW_PRESS || action == GLFW_REPEAT)
                    input->active[key] = true;
                else if (action == GLFW_RELEASE)
                    input->active[key] = false;

                // ESC: quit
                if (input->active[GLFW_KEY_ESCAPE])
                    quit = true;

                handler(input, dt);
            }

            void onMouseEvent(int button, int action, float dt) {
                // Register mouse button press
                if (action == GLFW_PRESS)
                    input->active[button] = true;
                else if (action == GLFW_RELEASE)
                    input->active[button] = false;

                handler(input, dt);
            }

            void onCursorEvent(double xpos, double ypos, float dt) {
                input->currentMousePos = {xpos, ypos};
                handler(input, dt);
            }

        public:

            Ref<Window> window;
            Ref<Input> input;

            bool quit;
            EventHandler handler;
    };
}