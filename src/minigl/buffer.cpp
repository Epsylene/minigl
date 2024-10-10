#include "buffer.hpp"

namespace minigl
{
    //----------- VERTEX BUFFER -----------//

    VertexBuffer::VertexBuffer(float* vertices, size_t size, DataUsage usage)
    {
        glCreateBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, (GLenum)usage);
    }

    VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, DataUsage usage)
    {
        glCreateBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], (GLenum)usage);

        layout = {{{DataType::Float3, "a_pos"},
                   {DataType::Float3, "a_normal"},
                   {DataType::Float2, "a_tex"},
                   {DataType::Float4, "a_color"}}};
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &bufferID);
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    }

    void VertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::update_vertices(const std::vector<Vertex>& vertices)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
    }

    //----------- INDEX BUFFER -----------//

    IndexBuffer::IndexBuffer(const uint32_t* indices, size_t count, DataUsage usage): count(count)
    {
        glCreateBuffers(1, &idxBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, (GLenum)usage);
    }

    IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices, DataUsage usage): count(indices.size())
    {
        glCreateBuffers(1, &idxBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], (GLenum)usage);
    }

    IndexBuffer::~IndexBuffer()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &idxBufferID);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufferID);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    //----------- VERTEX ARRAY -----------//

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &vtxArrID);
    }

    VertexArray::VertexArray(const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib)
    {
        glCreateVertexArrays(1, &vtxArrID);

        addVertexBuffer(vb);
        setIndexBuffer(ib);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(vtxArrID);

        for (auto& vb: vertexBuffers)
        {
            for (int i = 0; i < vb->getLayout().size(); ++i)
            {
                glEnableVertexAttribArray(i);
            }
        }
    }

    void VertexArray::unbind() const
    {
        for (auto& vb: vertexBuffers)
        {
            for (int i = 0; i < vb->getLayout().size(); ++i)
            {
                glDisableVertexAttribArray(i);
            }
        }

        glBindVertexArray(0);
    }

    void VertexArray::updateVertices(size_t buffer_idx, const std::vector<Vertex>& vertices)
    {
        glBindVertexArray(vtxArrID);
        vertexBuffers[buffer_idx]->update_vertices(vertices);
    }

    void VertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        glBindVertexArray(vtxArrID);
        vertexBuffer->bind();

        MGL_ASSERT(!vertexBuffer->getLayout().empty(), "Vertex buffer has no layout.")

        // For each element in the layout, enable the vertex
        // attribute array at the element's index and give it
        // the data it wants.
        const auto& layout = vertexBuffer->getLayout();
        for (uint32_t index = 0; const auto& element: layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.count(),
                                  dataTypeToGLType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  layout.stride,
                                  (const void*)element.offset);
            index++;
        }

        vertexBuffers.push_back(vertexBuffer);

        for (int i = 0; i < layout.size(); ++i)
        {
            glDisableVertexAttribArray(i);
        }
    }

    void VertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        this->bind();
        indexBuffer->bind();

        this->indexBuffer = indexBuffer;
        this->unbind();
    }

    FrameBuffer::FrameBuffer()
    {
        glCreateFramebuffers(1, &fboID);
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &fboID);
    }

    void FrameBuffer::attach_color_texture(const Ref<Texture>& color_texture)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fboID);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->id, 0);
        colorAttachment = color_texture;
    }

    void FrameBuffer::attach_depth_texture(const Ref<Texture>& depth_texture)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fboID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->id, 0);
        
        if (!colorAttachment) {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        
        depthAttachment = depth_texture;
    }

    void FrameBuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    }

    void FrameBuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}