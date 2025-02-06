#include "app.hpp"
#include "minigl/commands.hpp"

#include <glad/glad.h>

namespace minigl
{
    void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
    {
        auto const src_str = [source]() {
            switch (source)
            {
                case GL_DEBUG_SOURCE_API: return "API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
                case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
                case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
                case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
                case GL_DEBUG_SOURCE_OTHER: return "OTHER";
            }
        }();

        auto const type_str = [type]() {
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR: return "ERROR";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
                case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
                case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
                case GL_DEBUG_TYPE_MARKER: return "MARKER";
                case GL_DEBUG_TYPE_OTHER: return "OTHER";
            }
        }();

        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION: trace("[{}][{}] ({}): {}", src_str, type_str, id, message); break;
            case GL_DEBUG_SEVERITY_LOW: info("[{}][{}] ({}): {}", src_str, type_str, id, message); break;
            case GL_DEBUG_SEVERITY_MEDIUM: warn("[{}][{}] ({}): {}", src_str, type_str, id, message); break;
            case GL_DEBUG_SEVERITY_HIGH: error("[{}][{}] ({}): {}", src_str, type_str, id, message); break;
        }
    }

    App::App(const int width, const int height)
    {
        window = box<Window>(width, height);
        input = box<Input>(window->get_native_window());

        // Set the GLFW callbacks
        set_glfw_callbacks();

        // Set the debug callback
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debug_callback, nullptr);

        // Enable depth testing and depth clamping
        RenderCommand::set_depth_test(true);
        RenderCommand::set_depth_clamp(true);
        // RenderCommand::set_face_culling(true);
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

    float App::time()
    {
        return glfwGetTime();
    }

    void App::set_glfw_callbacks()
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