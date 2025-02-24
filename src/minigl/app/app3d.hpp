#pragma once

#include "app.hpp"
#include "minigl/app/camera.hpp"

namespace mgl
{
    class App3D: public App
    {
        public:

            App3D(std::string name, const int width, const int height):
                App(name, width, height)
            {
                event_handler([this](Ref<Input> input, float dt){
                    camera.onUpdate(input, dt);
                });
            }

        public:

            FreeCamera camera;
    };
}