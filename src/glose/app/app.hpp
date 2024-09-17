#pragma once

#include "core.hpp"
#include "window.hpp"
#include "input/input.hpp"

namespace glose
{
    class App 
    {
        public:

            App(const int width, const int height);
            virtual ~App() = default;

            void run();

            virtual void onUpdate(Ref<Input> input, float dt) {};
            virtual void render() {};

        private:

            void onWindowClose();
            void onWindowResize(int new_width, int new_height);

            Box<Window> window;
            Ref<Input> input;
            bool running = true, minimized = false;
            float lastFrameTime = 0.f;
    };
}