#include "render_command.hpp"

namespace minigl
{
    void RenderCommand::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderCommand::set_clear_color(const Color& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderCommand::set_depth_test(bool enabled)
    {
        enabled ? glEnable(GL_DEPTH_TEST)
                : glDisable(GL_DEPTH_TEST);
    }

    void RenderCommand::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void RenderCommand::draw_indexed(const Ref<VertexArray>& vertexArray,
                                    Primitives drawPrimitive)
    {
        glDrawElements((int)drawPrimitive,
                       vertexArray->getIndexBuffer()->getCount(),
                       GL_UNSIGNED_INT,
                       nullptr);
    }
}