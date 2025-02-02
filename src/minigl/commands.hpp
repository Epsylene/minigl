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
        LINES = GL_LINES,
        LINE_STRIP = GL_LINE_STRIP,
    };

    class Fence
    {
        public:

            void wait();
            void reset();

        private:

            GLsync fence;
    };

    /// Render commands: functionality to execute several
    /// OpenGL render commands, like clearing the window or
    /// drawing an indexed vertex array.
    struct RenderCommand
    {
        /// Clear the window. Clears by default the color and
        /// depth buffers.
        static void clear(GLenum flags = BufferBit::COLOR | BufferBit::DEPTH);

        /// Set the background clear color
        static void set_clear_color(const Color& color);

        /// Enable or disable depth testing
        static void set_depth_test(bool enabled);

        /// Enable or disable depth clamping
        static void set_depth_clamp(bool enabled);

        /// Enable or disable face culling
        static void set_face_culling(bool enabled);

        /// Set the rendering viewport
        ///
        /// @param x, y Coordinates of the top-left corner of the
        ///     rendering viewport
        /// @param width The rendering viewport's width
        /// @param height The rendering viewport's height
        static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        /// Enable or disable wireframe rendering
        static void wireframe(bool enabled);

        /// Draw a set of indexed vertices
        ///
        /// @param vertexArray The VA attached to the
        ///     vertices
        /// @param drawPrimitive The rendering
        ///     primitive (triangles, points, etc)
        static void draw_indexed(const Ref<VertexArray>& vertexArray, Primitives drawPrimitive = Primitives::TRIANGLES);

        static void draw_instanced(const Ref<VertexArray>& vertexArray, uint32_t count, Primitives drawPrimitive = Primitives::TRIANGLES);

        static void draw_indirect(const Ref<VertexArray>& vertexArray, uint32_t count, Primitives drawPrimitive = Primitives::TRIANGLES);

        /// Launch a set of (numGroupsX, numGroupsY,
        /// numGroupsZ) compute work groups in each dimension.
        static void dispatch_compute(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ);

        /// Set a barrier to synchronize memory accesses
        /// relative to all possible shader operations. That
        /// is, block accesses until all operations are
        /// completed.
        static void memory_barrier();
    };
}