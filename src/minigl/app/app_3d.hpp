#pragma once

#include "app.hpp"
#include "minigl/app/camera.hpp"

namespace minigl
{
    class App3D: public App
    {
        public:

            App3D(const int width, const int height):
                camera(), App(width, height)
            {}

            virtual void onUpdate(float dt) override
            {
                camera.onUpdate(input, dt);
            }

        protected:

            Camera camera;
    };
}