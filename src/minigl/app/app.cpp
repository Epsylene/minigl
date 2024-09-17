#include "app.hpp"
#include "minigl/render_command.hpp"

#include <glad/glad.h>

namespace minigl
{
    App::App(const int width, const int height)
    {
        window = box<Window>(width, height);
        input = box<Input>(window->get_native_window());

        glfwSetWindowUserPointer(window->get_native_window(), this);

        glfwSetWindowCloseCallback(window->get_native_window(), [](GLFWwindow* window) {
            App* app = (App*)glfwGetWindowUserPointer(window);
            app->onWindowClose();
        });

        glfwSetWindowSizeCallback(window->get_native_window(), [](GLFWwindow* window, int width, int height) {
            App* app = (App*)glfwGetWindowUserPointer(window);
            app->onWindowResize(width, height);
        });
    }

    void App::onWindowClose()
    {
        running = false;
    }

    void App::onWindowResize(int new_width, int new_height)
    {
        window->set_viewport(new_width, new_height);

        if(new_width == 0 || new_height == 0)
        {
            minimized = true;
        }
        minimized = false;
    }

    void App::run()
    {
        while (running) {
            auto time = (float)glfwGetTime();
            float dt = time - lastFrameTime;
            lastFrameTime = time;

            if(!minimized) {
                // Clear the screen
                RenderCommand::setClearColor({0.2f});
                RenderCommand::clear();

                // Update and draw stuff
                onUpdate(input, dt);
                render();
            }

            // Poll events and swap buffers
            glfwPollEvents();
            glfwSwapBuffers(window->get_native_window());
        }
    }
}