#pragma once

#include "core.hpp"
#include "window.hpp"
#include "input/input.hpp"

namespace minigl
{
    class App 
    {
        public:

            App(const int width, const int height);
            virtual ~App() = default;

            void run();

            virtual void onUpdate(float dt) {};
            virtual void render() {};

        protected:
        
            Ref<Input> input;
            float dt = 0.f;
        
        private:

            void onWindowClose();
            void onWindowResize(int new_width, int new_height);

            void set_glfw_callbacks();

            Box<Window> window;
            bool running = true, minimized = false;
            float lastFrameTime = 0.f;
    };
}