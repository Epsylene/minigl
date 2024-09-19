#include "mglpch.hpp"
#include "color.hpp"
#include "buffer.hpp"

#include <glad/glad.h>

namespace minigl
{
    enum class Primitives
    {
        TRIANGLES = GL_TRIANGLES,
        POINTS = GL_POINTS,
        LINES = GL_LINES
    };

    /// @brief OpenGL render commands class
    ///
    /// This class provides functions to execute several OpenGL
    /// render commands, like clearing the window or drawing an
    /// indexed vertex array.
    class RenderCommand
    {
        public:
            /// @brief Clear the window
            ///
            /// @details By default, clears the color and depth
            /// buffers.
            static void clear();

            /// @brief Set the background clear color
            static void set_clear_color(const Color& color);

            /// @brief Enable or disable depth testing
            static void set_depth_test(bool enabled);

            /// @brief Set the rendering viewport
            ///
            /// @param x, y Coordinates of the top-left corner of the
            ///     rendering viewport
            /// @param width The rendering viewport's width
            /// @param height The rendering viewport's height
            static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

            /// @brief Draw a set of vertices
            ///
            /// @param vertexArray The VA attached to the
            ///     vertices
            /// @param drawPrimitive The rendering
            ///     primitive (triangles, points, etc)
            ///
            /// @see Primitives
            static void draw_indexed(const Ref<VertexArray>& vertexArray, Primitives drawPrimitive = Primitives::TRIANGLES);
    };
}