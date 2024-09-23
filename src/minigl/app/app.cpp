#include "app.hpp"
#include "minigl/render_command.hpp"

#include <glad/glad.h>

namespace minigl
{
    App::App(const int width, const int height)
    {
        window = box<Window>(width, height);
        input = box<Input>(window->get_native_window());

        // Set the GLFW callbacks
        set_callbacks();

        // Enable depth testing and depth clamping
        RenderCommand::set_depth_test(true);
        RenderCommand::set_depth_clamp(true);
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
            // Update dt
            auto time = (float)glfwGetTime();
            dt = time - lastFrameTime;
            lastFrameTime = time;

            if(!minimized) {
                // Clear the screen
                RenderCommand::set_clear_color({0.2f});
                RenderCommand::clear();

                // Update and draw stuff
                onUpdate(dt);
                render();
            }

            // Poll events and swap buffers
            glfwPollEvents();
            glfwSwapBuffers(window->get_native_window());
        }
    }

    void App::set_callbacks()
    {
        // App pointer for the callbacks
        glfwSetWindowUserPointer(window->get_native_window(), this);

        // Window close callback
        glfwSetWindowCloseCallback(window->get_native_window(), [](GLFWwindow* window) {
            auto app = (App*)glfwGetWindowUserPointer(window);
            app->onWindowClose();
        });

        // Window resize callback
        glfwSetWindowSizeCallback(window->get_native_window(), [](GLFWwindow* window, int width, int height) {
            auto app = (App*)glfwGetWindowUserPointer(window);
            app->onWindowResize(width, height);
        });
    }
}