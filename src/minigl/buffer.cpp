#include "buffer.hpp"

namespace minigl
{
    //----------- VERTEX BUFFER -----------//

    void VertexBuffer::create_buffer(const void* data, size_t size, DataAccess usage)
    {
        glCreateBuffers(1, &bufferID);
        glNamedBufferStorage(bufferID, size, data, (GLenum)usage);
    }

    VertexBuffer::VertexBuffer(float* vertices, size_t size, DataAccess usage)
    {
        create_buffer(vertices, size, usage);

        count = size / sizeof(float);
        layout = {{{DataType::Float3, "a_pos"},
                   {DataType::Float3, "a_normal"},
                   {DataType::Float2, "a_tex"},
                   {DataType::Float4, "a_color"}}};
    }

    VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, DataAccess usage)
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

    IndexBuffer::IndexBuffer(const uint32_t* indices, size_t count, DataAccess usage): count(count)
    {
        glCreateBuffers(1, &idxBufferID);
        glNamedBufferStorage(idxBufferID, count * sizeof(uint32_t), indices, (GLenum)usage);
    }

    IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices, DataAccess usage): count(indices.size())
    {
        glCreateBuffers(1, &idxBufferID);
        glNamedBufferStorage(idxBufferID, indices.size() * sizeof(uint32_t), indices.data(), (GLenum)usage);
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

    void FrameBuffer::add_color_attachment(const Ref<Texture>& color_texture)
    {
        auto index = colorAttachments.size();
        glNamedFramebufferDrawBuffer(fboID, GL_COLOR_ATTACHMENT0 + index);
        glNamedFramebufferTexture(fboID, GL_COLOR_ATTACHMENT0 + index, color_texture->id, 0);
        colorAttachments.push_back(color_texture);
    }

    void FrameBuffer::set_depth_attachment(const Ref<Texture>& depth_texture)
    {
        glNamedFramebufferTexture(fboID, GL_DEPTH_ATTACHMENT, depth_texture->id, 0);
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

    IndirectBuffer::IndirectBuffer(const std::vector<DrawCommand>& commands, DataAccess usage)
    {
        glCreateBuffers(1, &indirectBufferID);
        
        size = commands.size() * sizeof(DrawCommand);
        glNamedBufferStorage(indirectBufferID, size, commands.data(), (GLenum)usage);

        if (usage & DataAccess::MapRead || usage & DataAccess::MapWrite)
            mappedBuffer = (DrawCommand*)glMapNamedBufferRange(indirectBufferID, 0, size, (GLenum)usage);
    }

    IndirectBuffer::~IndirectBuffer()
    {
        mappedBuffer = nullptr;
        glUnmapBuffer(indirectBufferID);
        glDeleteBuffers(1, &indirectBufferID);
    }

    void IndirectBuffer::bind() const
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBufferID);
    }

    void IndirectBuffer::write(size_t index, const DrawCommand& command)
    {
        mappedBuffer[index] = command;
    }
    
    const DrawCommand& IndirectBuffer::read(size_t index) const
    {
        return mappedBuffer[index];
    }

    void IndirectBuffer::flush()
    {
        glFlushMappedNamedBufferRange(indirectBufferID, 0, size);
    }

    //----------- UNIFORM BUFFER -----------//

    void UniformBuffer::create_buffer(const void* data, size_t size, uint32_t binding_point, DataAccess usage)
    {
        glCreateBuffers(1, &uboID);
        glNamedBufferStorage(uboID, size, data, (GLenum)usage);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, uboID);
    }

    void UniformBuffer::bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboID);
    }
}