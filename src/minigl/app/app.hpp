#pragma once

#include "core.hpp"

#include "window.hpp"
#include "input/input.hpp"
#include "event.hpp"

namespace mgl
{
    class App 
    {
        public:

            App(const int width, const int height);
            virtual ~App() = default;

            void run();

            virtual void onUpdate(float dt) {};
            virtual void render() {};

            /// Time since the application started, in seconds.
            static float time();

        protected:
        
            float dt = 0.f;
            Ref<Event> event;
        
        private:

            void set_glfw_callbacks();
            Ref<Window> window;

            float lastFrameTime = 0.f;
    };
}