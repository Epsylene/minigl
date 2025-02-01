#include "commands.hpp"

namespace minigl
{
    void Fence::wait()
    {
        // Reset the fence if it is signaled
        fence = 0;

        // Check once making sure to flush the command queue,
        // then loop until the fence is signaled.
        GLenum wait_return = glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
        while (wait_return == GL_ALREADY_SIGNALED || wait_return == GL_CONDITION_SATISFIED) {
            wait_return = glClientWaitSync(fence, 0, 0);
        }
    }

    void Fence::reset()
    {
        fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }

    void RenderCommand::clear(GLenum bits)
    {
        glClear(bits);
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

    void RenderCommand::set_depth_clamp(bool enabled)
    {
        enabled ? glEnable(GL_DEPTH_CLAMP)
                : glDisable(GL_DEPTH_CLAMP);
    }

    void RenderCommand::set_face_culling(bool enabled)
    {
        enabled ? glEnable(GL_CULL_FACE)
                : glDisable(GL_CULL_FACE);
    }

    void RenderCommand::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void RenderCommand::wireframe(bool enabled)
    {
        enabled ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
                : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void RenderCommand::draw_indexed(const Ref<VertexArray>& vertexArray,
                                    Primitives drawPrimitive)
    {
        glDrawElements((GLenum)drawPrimitive,
                       vertexArray->index_count(),
                       GL_UNSIGNED_INT,
                       nullptr);
    }

    void RenderCommand::draw_instanced(const Ref<VertexArray>& vertexArray,
                                      uint32_t count,
                                      Primitives drawPrimitive)
    {
        glDrawElementsInstanced((GLenum)drawPrimitive,
                                vertexArray->index_count(),
                                GL_UNSIGNED_INT,
                                nullptr,
                                count);
    }

    void RenderCommand::draw_indirect(const Ref<VertexArray>& vertexArray,
                                    uint32_t count,
                                    Primitives drawPrimitive)
    {
        glMultiDrawElementsIndirect((GLenum)drawPrimitive,
                                    GL_UNSIGNED_INT,
                                    nullptr,
                                    count,
                                    0);
    }

    void RenderCommand::dispatch_compute(uint32_t numGroupsX, uint32_t numGroupsY,
                                       uint32_t numGroupsZ)
    {
        glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
    }

    void RenderCommand::memory_barrier()
    {
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
}