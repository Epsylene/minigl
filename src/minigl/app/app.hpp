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

            App(std::string name, const int width, const int height);
            virtual ~App() = default;

            void event_handler(EventHandler handler) {
                events->handler = handler;
            }
            
            void run(std::function<void()> loop);
            
            /// Time since the application started, in seconds.
            static float time();
            
        protected:
            
            float dt = 0.f;
            float lastFrameTime = 0.f;
            
            Ref<Window> window;
            Ref<Event> events;
            Ref<Input> input;
                        
        private:
            
            void set_glfw_callbacks();
    };
}