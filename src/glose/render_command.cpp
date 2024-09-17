#include "render_command.hpp"

namespace glose
{
    void RenderCommand::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderCommand::setClearColor(const Color& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderCommand::setViewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void RenderCommand::drawIndexed(const Ref<VertexArray>& vertexArray,
                                    Primitives drawPrimitive)
    {
        glDrawElements((int)drawPrimitive,
                       vertexArray->getIndexBuffer()->getCount(),
                       GL_UNSIGNED_INT,
                       nullptr);
    }
}