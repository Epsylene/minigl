#include "app.hpp"
#include "minigl/commands.hpp"

#include <glad/glad.h>

namespace mgl
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

            return "UNKNOWN";
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

            return "UNKNOWN";
        }();

        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION: trace("[{}][{}] ({}): {}", src_str, type_str, id, message); break;
            case GL_DEBUG_SEVERITY_LOW: info("[{}][{}] ({}): {}", src_str, type_str, id, message); break;
            case GL_DEBUG_SEVERITY_MEDIUM: warn("[{}][{}] ({}): {}", src_str, type_str, id, message); break;
            case GL_DEBUG_SEVERITY_HIGH: error("[{}][{}] ({}): {}", src_str, type_str, id, message); break;
        }
    }

    App::App(std::string name, const int width, const int height)
    {
        window = ref<Window>(width, height);
        event = ref<Event>(window);

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

    void App::run()
    {
        while (!event->quit) {
            // Update dt
            auto time = (float)glfwGetTime();
            dt = time - lastFrameTime;
            lastFrameTime = time;

            if(!window->minimized) {
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
        auto _window = window->get_native_window();

        // App pointer for the callbacks
        glfwSetWindowUserPointer(_window, this);
        #define event(callback) \
            auto app = (App*)glfwGetWindowUserPointer(window); \
            app->event->callback;

        // Window close callback
        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            event(onWindowClose());
        });

        // Window resize callback
        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            event(onWindowResize(width, height));
        });

        // Key input callback
        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            event(onKeyEvent(key, scancode, action, mods));
        });

        // Mouse button callback
        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            event(onMouseEvent(button, action, mods));
        });

        // Mouse cursor callback
        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos) {
            event(onCursorEvent(xpos, ypos));
        });
    }
}