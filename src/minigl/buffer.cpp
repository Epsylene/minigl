#include "buffer.hpp"

namespace minigl
{
    //----------- VERTEX BUFFER -----------//

    void VertexBuffer::create_buffer(const void* data, size_t size, DataUsage usage)
    {
        glCreateBuffers(1, &bufferID);
        glNamedBufferStorage(bufferID, size, data, GL_DYNAMIC_STORAGE_BIT);
    }

    VertexBuffer::VertexBuffer(float* vertices, size_t size, DataUsage usage)
    {
        create_buffer(vertices, size, usage);

        count = size / sizeof(float);
        layout = {{{DataType::Float3, "a_pos"},
                   {DataType::Float3, "a_normal"},
                   {DataType::Float2, "a_tex"},
                   {DataType::Float4, "a_color"}}};
    }

    VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, DataUsage usage)
    {
        create_buffer(vertices.data(), vertices.size() * sizeof(Vertex), usage);

        count = vertices.size();
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
        glNamedBufferStorage(idxBufferID, count * sizeof(uint32_t), indices, (GLenum)usage);
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

        add_vertex_buffer(vb);
        set_index_buffer(ib);

        glBindVertexArray(0);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(vtxArrID);
        for (auto& vb: vertexBuffers)
        {
            vb->bind();
        }
    }

    void VertexArray::unbind() const
    {
        for (auto& vb: vertexBuffers)
        {
            vb->unbind();
        }
        glBindVertexArray(0);
    }

    void VertexArray::add_vertex_buffer(const Ref<VertexBuffer>& vb)
    {
        int binding = 0;
        for (auto& vertexBuffer: vertexBuffers)
        {
            binding += vertexBuffer->attribute_count();
        }

        // The vertex array is provided the ID of the new
        // vertex buffer, which is bound at the index
        // corresponding to the next possible location after
        // the previous buffers (for example, 2 if a pos|normal
        // buffer is already present), and the stride of the
        // internal layout of the new buffer.
        glVertexArrayVertexBuffer(vtxArrID, binding, vb->bufferID, 0, vb->layout.stride);

        // For each element in the layout:
        const auto& layout = vb->layout; 
        for (uint32_t attribute = binding; const auto& element: layout)
        {
            // - Enable the attribute
            glEnableVertexArrayAttrib(vtxArrID, attribute);
            
            // - Set its format (size, type, normalization,
            //   offset)
            glVertexArrayAttribFormat(
                vtxArrID,
                attribute,
                element.count(),
                dataTypeToGLType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                element.offset
            );

            // - Set the binding divisor for instanced/indirect
            //   rendering
            glVertexArrayBindingDivisor(
                vtxArrID,
                binding,
                element.divisor
            );

            // - Bind the attribute to the currently bound
            //   vertex buffer
            glVertexArrayAttribBinding(
                vtxArrID, 
                attribute, 
                binding
            );

            attribute++;
        }

        vertexBuffers.push_back(vb);
    }

    void VertexArray::set_index_buffer(const Ref<IndexBuffer>& ib)
    {
        glVertexArrayElementBuffer(vtxArrID, ib->idxBufferID);
        indexBuffer = ib;
    }

    void VertexArray::updateVertices(size_t index, const std::vector<Vertex>& vertices)
    {
        glBindVertexArray(vtxArrID);
        vertexBuffers[index]->update_vertices(vertices);
    }

    //----------- FRAMEBUFFER -----------//

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

    //----------- INDIRECT BUFFER -----------//

    IndirectBuffer::IndirectBuffer(const std::vector<DrawIndirectCommand>& commands)
    {
        glCreateBuffers(1, &indirectBufferID);
        glNamedBufferStorage(indirectBufferID, commands.size() * sizeof(DrawIndirectCommand), commands.data(), 0);
    }

    void IndirectBuffer::bind() const
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBufferID);
    }

    //----------- UNIFORM BUFFER -----------//

    void UniformBuffer::create_buffer(const void* data, size_t size, uint32_t binding_point, DataUsage usage)
    {
        glCreateBuffers(1, &uboID);
        glNamedBufferStorage(uboID, size, data, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, uboID);
    }

    void UniformBuffer::bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboID);
    }
}