#pragma once

#include "core.hpp"
#include "window.hpp"

namespace glose
{
    class App 
    {
        public:

            App(const int width, const int height);
            virtual ~App() = default;

            void run();
            virtual void render() {};

        private:

            void onWindowClose();
            void onWindowResize(int new_width, int new_height);

            Box<Window> window;
            bool running = true, minimized = false;
    };
}