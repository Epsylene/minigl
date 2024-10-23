#pragma once

#include "core.hpp"
#include "mglpch.hpp"
#include "geometry.hpp"
#include "color.hpp"
#include "texture.hpp"

#include <glad/glad.h>

namespace minigl
{
    /// Buffer data type abstraction
    enum class DataType
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Mat2, Mat4
    };

    /// Get the DataType type size in bytes
    static uint32_t dataTypeSize(DataType type)
    {
        switch (type)
        {
            case DataType::Float: return 4;
            case DataType::Float2: return 4 * 2;
            case DataType::Float3: return 4 * 3;
            case DataType::Float4: return 4 * 4;

            case DataType::Int: return 4;
            case DataType::Int2: return 4 * 2;
            case DataType::Int3: return 4 * 3;
            case DataType::Int4: return 4 * 4;

            case DataType::Mat2: return 4 * 2 * 2;
            case DataType::Mat4: return 4 * 4 * 4;

            case DataType::None: break;
        }

        MGL_ASSERT(false, "Unknown shader data type.");
        return -1;
    }

    /// Get the OpenGL data type from the DataType
    static GLenum dataTypeToGLType(DataType type)
    {
        switch (type)
        {
            case DataType::Float: return GL_FLOAT;
            case DataType::Float2: return GL_FLOAT;
            case DataType::Float3: return GL_FLOAT;
            case DataType::Float4: return GL_FLOAT;

            case DataType::Int: return GL_INT;
            case DataType::Int2: return GL_INT;
            case DataType::Int3: return GL_INT;
            case DataType::Int4: return GL_INT;

            case DataType::Mat2: break;
            case DataType::Mat4: break;

            case DataType::None: break;
        }

        MGL_ASSERT(false, "Unknown shader data type.");
        return -1;
    }

    /// Group of tightly packed data in the vertex buffer at a
    /// given offset, corresponding to an attribute.
    struct BufferElement
    {
        /// Name of the attribute (for debug purposes)
        std::string name;
        /// Type of the data in the element
        DataType type;
        /// Offset of the element in the buffer
        uint32_t offset = 0;
        /// Size of the element in bytes
        size_t size;
        /// If true, values stored in an integer format will be
        /// normalized to a value in the range [-1, 1] (signed)
        /// or [0, 1] (unsigned).
        bool normalized;
        /// When using indirect rendering, the divisor
        /// specifies the instancing rate of the buffer
        /// element: if `divisor` is 0, the element is applied
        /// once per vertex; if it is 1, the element is applied
        /// once per instance; if it is 2, once every two
        /// instances, etc.
        uint32_t divisor = 0;

        BufferElement() = default;

        BufferElement(DataType type, const std::string& name, uint32_t divisor = 0, bool normalized = false):
            name(name), type(type), size(dataTypeSize(type)), normalized(normalized), divisor(divisor) {}

        /// Number of components of the underlying data type of
        /// the vertex buffer element
        uint32_t count() const
        {
            switch (type)
            {
                case DataType::Float: return 1;
                case DataType::Float2: return 2;
                case DataType::Float3: return 3;
                case DataType::Float4: return 4;

                case DataType::Int: return 1;
                case DataType::Int2: return 2;
                case DataType::Int3: return 3;
                case DataType::Int4: return 4;

                case DataType::None: break;
            }

            MGL_ASSERT(false, "Unknown shader or data type not supported.");
            return -1;
        }
    };

    /// Vertex buffer layout abstraction: a buffer is
    /// structured as a list of BufferElement objects, each of
    /// which correspond to a buffer attribute.
    struct BufferLayout
    {
        /// Attributes of the vertex buffer
        std::vector<BufferElement> elements;
        /// Distance between consecutive attribute components.
        uint32_t stride;

        BufferLayout() = default;

        /// A buffer layout is a list of same-type data groups,
        /// which are given an explanatory name.
        BufferLayout(const std::initializer_list<BufferElement>& elements): elements(elements)
        {
            uint32_t offset = 0;
            stride = 0;

            for (auto& e: this->elements)
            {
                // Offset is where an element group starts, stride
                // the distance to the next same element group.
                e.offset = offset;
                stride += e.size;
                offset += e.size;
            }
        }

        std::vector<BufferElement>::iterator begin() { return elements.begin(); }
        std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
        std::vector<BufferElement>::iterator end() { return elements.end(); }
        std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

        bool empty() const { return elements.empty(); }
        size_t size() const { return elements.size(); };
    };

    /// Generic buffer of vertices with a custom layout
    template<typename vertex_t>
    using Buffer = std::vector<vertex_t>;

    /// Vertex struct: a vertex is comprised of a position, a
    /// normal, a texture coordinate, and a color.
    struct Vertex
    {
        Vec3 pos;
        Vec3 normal;
        Vec2 tex;
        Color color;
    };

    /// How the data of the mesh is supposed to be used during
    /// rendering
    enum class DataUsage
    {
        /// Loaded and then unchanged. Most common usage.
        Static = GL_STATIC_DRAW,
        /// Loaded and then occasionaly modified.
        Dynamic = GL_DYNAMIC_DRAW,
        /// Modified repeatedly (every frame).
        Stream = GL_STREAM_DRAW,
    };

    /// The VBO (Vertex Buffer Object) is a buffer containing
    /// data for a set of vertices. Binding the vertex buffer
    /// to OpenGL allows sending large batches of data all at
    /// once to the graphics card, that will be kept in the GPU
    /// memory, where they can be accessed faster.
    class VertexBuffer
    {
        public:

            uint32_t bufferID;
            uint32_t count;
            BufferLayout layout;

            void create_buffer(const void* data, size_t size, DataUsage usage);

        public:

            /// Creates a vertex buffer from an array of
            /// floats. The vertex buffer is created and bound
            /// to OpenGL, with usage set to `Static` by
            /// default (see `DataUsage`).
            ///
            /// @param vertices The array of vertices
            /// @param size The array size in bytes (`sizeof()`)
            VertexBuffer(float* vertices, size_t size, DataUsage usage = DataUsage::Static);

            /// Create a vertex buffer from an array of
            /// vertices. The vertex buffer is created and
            /// bound to OpenGL, with usage set to `Static` by
            /// default (see `DataUsage`). Its layout is set to
            /// `[pos, normal, tex, color]`.
            explicit VertexBuffer(const std::vector<Vertex>& vertices, DataUsage usage = DataUsage::Static);

            /// Create a vertex buffer from a buffer of
            /// vertices with a custom layout.
            template<typename vertex_t>
            VertexBuffer(const Buffer<vertex_t>& vertices, const BufferLayout& layout, DataUsage usage = DataUsage::Static) {
                create_buffer(vertices.data(), vertices.size() * sizeof(vertex_t), usage);
                count = vertices.size();
                this->layout = layout;
            }

            /// Destructor. Calls `glDelete()` over the vertex
            /// buffer.
            virtual ~VertexBuffer();

            void bind() const;
            void unbind() const;

            uint32_t vertex_count() const { return count; }
            uint32_t attribute_count() const { return layout.size(); }

            /// Update the vertices of the vertex buffer.
            void update_vertices(const std::vector<Vertex>& vertices);

            virtual const BufferLayout& getLayout() const { return layout; }
            virtual void setLayout(const BufferLayout& layout) { this->layout = layout; }
    };

    /// Meshes are formed of triangles. A single VBO for a
    /// given mesh is supposed to contain data for each vertex
    /// of each triangle, but this approach is not efficient:
    /// vertices shared by triangles are repeated several
    /// times. To avoid this overlap, index buffers provide as
    /// their name suggests a list of indices for each triangle
    /// vertex. Take for example the rectangle, which is
    /// comprised of two triangles. Instead of writing a VBO
    /// with two triangles sharing vertices on one side, we are
    /// providing a buffer containing only the actual vertices,
    /// and an index buffer looking like {0, 1, 3, 1, 2, 3}:
    /// with this buffer, OpenGL knows which vertex in the VBO
    /// (the position listed by the index) correspond to the
    /// current triangle vertex; thus, a vertex shared by two
    /// triangles is given the same index twice, and the VBO
    /// contents match the mesh data.
    class IndexBuffer
    {
        public:

            uint32_t idxBufferID;
            uint32_t count;

        public:

            /// The index buffer is created and bound to
            /// OpenGL, with usage set to `Static` by default
            /// (see `DataUsage`).
            IndexBuffer(const uint32_t* indices, size_t count, DataUsage usage = DataUsage::Static);

            /// Create an index buffer from a vector of
            /// indices. The index buffer is created and bound
            /// to OpenGL, with usage set to `Static` by
            /// default (see `DataUsage`).
            explicit IndexBuffer(const std::vector<uint32_t>& indices, DataUsage usage = DataUsage::Static);

            /// Destructor. Calls `glDelete()` over the index
            /// buffer.
            virtual ~IndexBuffer();

            void bind() const;
            void unbind() const;

            inline uint32_t getCount() const { return count; }
    };

    /// The VAO (Vertex Array Object) is a buffer containing
    /// attribute calls to a VBO; that is, if the vertex buffer
    /// structure is `(pos|tex|normal)`, the VAO will provide
    /// function calls to the GPU to access first the `pos`
    /// attributes, then `tex`, then `normal`, without having
    /// to check with the CPU each time. Core OpenGL requires
    /// binding a VAO to draw vertex data. Apart from that, the
    /// class attaches an index buffer to the VBO, and provides
    /// functions to update the vertices of the VBO.
    class VertexArray
    {
        public:

            VertexArray();
            VertexArray(const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib);

            void bind() const;
            void unbind() const;

            void add_vertex_buffer(const Ref<VertexBuffer>& vb);
            void set_index_buffer(const Ref<IndexBuffer>& ib);

            inline uint32_t getCount() const { return indexBuffer->getCount(); }

            /// Update the vertices of the vertex buffer.
            void updateVertices(size_t index, const std::vector<Vertex>& vertices);

        private:

            uint32_t vtxArrID;
            std::vector<Ref<VertexBuffer>> vertexBuffers;
            Ref<IndexBuffer> indexBuffer;
    };

    class FrameBuffer
    {
        public:

            FrameBuffer();
            virtual ~FrameBuffer();

            void attach_color_texture(const Ref<Texture>& texture);
            void attach_depth_texture(const Ref<Texture>& texture);

            void bind() const;
            void unbind() const;

        private:

            uint32_t fboID;
            Ref<Texture> colorAttachment;
            Ref<Texture> depthAttachment;
    };
}