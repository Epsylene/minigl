#pragma once

#include "core.hpp"
#include "mglpch.hpp"
#include "geometry.hpp"
#include "color.hpp"

#include <glad/glad.h>

namespace minigl
{
    /// @brief DataType
    ///
    /// @details This is a helper class, used to build the
    /// BufferElement objects. Each DataType represents an
    /// abstract type (float, int, bool, etc) which is then
    /// provided to OpenGL as a number of bytes.
    ///
    /// @see BufferLayout
    enum class DataType: uint8_t
    {
            None = 0,
            Float, Float2, Float3, Float4,
            Int, Int2, Int3, Int4,
            Mat2, Mat4
    };

    /// @brief Get the DataType type size in bytes
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

    /// @brief OpenGL vertex buffer element abstraction
    ///
    /// @details This is a helper class, used to build the BufferLayout
    /// objects. Each BufferElement represents a group of
    /// tightly packed, same-type data in the vertex buffer (for
    /// example, a group of four floats or one of three
    /// integers).
    ///
    /// @see BufferLayout
    struct BufferElement
    {
        std::string name;
        std::vector<DataType> types;
        uint32_t arrayNb = 1;
        uint32_t offset = 0;
        size_t size;
        bool normalized;

        BufferElement() = default;

        BufferElement(DataType type, const std::string& name, bool normalized = false):
            name(name), types({type}), size(dataTypeSize(type)), normalized(normalized) {}

        /// @brief Vertex buffer element constructor
        ///
        /// @param types The buffer element types
        /// @param name The buffer element name
        /// @param normalized Normalize fixed-point data
        /// values (default to false)
        BufferElement(const std::vector<DataType>& types, const std::string& name, size_t arrayNb = 1):
            name(name), types(types), size(0), normalized(false), arrayNb(arrayNb)
        {
            for (auto& type: types)
            {
                size += dataTypeSize(type);
            }

            size *= arrayNb;
        }

        /// @brief Number of elements in the underlying data
        /// type of the vertex buffer element
        ///
        /// @see DataType
        uint32_t count() const
        {
            if(types.size() == 1)
                switch (*types.begin())
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

    /// @brief Vertex buffer layout abstraction class
    ///
    /// @details This class intends to present the vertex buffer
    /// layout in a simple and easy-to-understand fashion,
    /// asking to provide an initializer list of BufferElement
    /// objects, which comprise a DataType and a string with the
    /// name of the element group (for example, "position" or
    /// "color").
    ///
    /// @see BufferElement, DataType
    class BufferLayout
    {
        private:

            std::vector<BufferElement> elements;
            uint32_t stride;

        public:

            BufferLayout() = default;

            /// @brief Get the vertex buffer elements
            inline const std::vector<BufferElement>& getElements() const { return elements; }

            /// @brief BufferLayout constructor
            ///
            /// @details A buffer layout is a list of same-type
            /// data groups, which are given an explanatory
            /// name.
            ///
            /// @param elements The list of BufferElement objects
            /// @see BufferElement
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

            /// @brief Get the distance between the elements of the
            ///  vertex buffer
            inline uint32_t getStride() const { return stride; }

            std::vector<BufferElement>::iterator begin() { return elements.begin(); }
            std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
            std::vector<BufferElement>::iterator end() { return elements.end(); }
            std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

            size_t size() const { return elements.size(); };
    };

    /// Vertex struct: a vertex is comprised of a position, a
    /// normal, a texture coordinate, and a color (white by
    /// default).
    struct Vertex
    {
        Vec3 pos, normal;
        Vec2 texCoord;
        Color color;

        Vertex(const Vec3& pos, const Vec3& normal, const Vec2& texCoord,
                const Color& color = Color::White):
            pos(pos), normal(normal), texCoord(texCoord), color(color) {}
    };

    /// @brief Vertex buffer abstraction class
    ///
    /// @details The VBO (Vertex Buffer Object) is a buffer
    /// containing the data for a set of vertices. Binding the
    /// vertex buffer to OpenGL allows sending large batches of
    /// data all at once to the graphics card, that will be kept
    /// in the GPU memory, where they can be accessed faster.
    class VertexBuffer
    {
        private:

            uint32_t bufferID;
            BufferLayout layout;

        public:

            /// @brief Creates a vertex buffer from an array of floats
            ///
            /// @details The vertex buffer is created and bound
            /// to OpenGL, with usage set to `GL_STATIC_DRAW`
            /// (modified once, drawn multiple times).
            ///
            /// @param vertices The array of vertices
            /// @param size The array size in bytes (`sizeof()`)
            VertexBuffer(float* vertices, size_t size);

            /// @brief Create a vertex buffer from an array of vertices
            ///
            /// @details The vertex buffer is created and bound
            /// to OpenGL, with usage set to `GL_STATIC_DRAW`
            /// (modified once, drawn multiple times). Its
            /// layout is set to [pos, normal, texCoords].
            ///
            /// @param vertices The array of vertices
            /// @see Vertex struct
            explicit VertexBuffer(const std::vector<Vertex>& vertices);

            /// @brief VertexBuffer destructor
            /// 
            /// @details Calls glDelete() over the vertex
            /// buffer.
            virtual ~VertexBuffer();

            /// @brief Bind the vertex buffer
            void bind() const;

            /// @brief Unbind the vertex buffer
            void unbind() const;

            /// @brief Get the vertex buffer layout
            /// @see BufferLayout
            virtual const BufferLayout& getLayout() const { return layout; }

            /// @brief Set the vertex buffer layout
            /// @see BufferLayout
            virtual void setLayout(const BufferLayout& layout) { this->layout = layout; }
    };

    /// @brief Index buffer abstraction class
    ///
    /// @details Meshes are formed of triangles. A single VBO
    /// for a given mesh is supposed to contain data for each
    /// vertex of each triangle, but this approach is not
    /// efficient: vertices shared by triangles are repeated
    /// several times. To avoid this overlap, index buffers
    /// provide as their name suggests a list of indices for
    /// each triangle vertex. Take for example the rectangle,
    /// which is comprised of two triangles. Instead of writing
    /// a VBO with two triangles sharing vertices on one side,
    /// we are providing a buffer containing only the actual
    /// vertices, and an index buffer looking like {0, 1, 3, 1,
    /// 2, 3}: with this buffer, OpenGL knows which vertex in
    /// the VBO (the position listed by the index) correspond
    /// to the current triangle vertex; thus, a vertex shared
    /// by two triangles is given the same index twice, and the
    /// VBO contents match the mesh data.
    class IndexBuffer
    {
        private:

            uint32_t idxBufferID;
            uint32_t count;

        public:

            /// @brief Index buffer constructor
            /// 
            /// @details The index buffer is created and bound
            /// to OpenGL, with usage set to GL_STATIC_DRAW
            /// (modified once, drawn multiple times).
            ///
            /// @param indices The vertex indices array
            /// @param count The array number of elements
            IndexBuffer(const uint32_t* indices, size_t count);

            explicit IndexBuffer(const std::vector<uint32_t>& indices);

            /// @brief Index Buffer destructor
            virtual ~IndexBuffer();

            /// @brief Bind the index buffer
            void bind() const;

            /// @brief Unbind the index buffer
            void unbind() const;

            /// @brief Get the index buffer array size (in
            /// number of elements)
            inline uint32_t getCount() const { return count; }
    };

    /// @brief Vertex array abstraction class
    ///
    /// @details The VAO (Vertex Array Object) is a buffer
    /// containing attribute calls to a VBO; that is, if the
    /// vertex buffer structure is `(pos|tex|normal)`, the VAO
    /// will provide function calls to the GPU to access first
    /// the `pos` attributes, then `tex`, then `normal`,
    /// without having to check with the CPU each time. Core
    /// OpenGL requires binding a VAO to draw vertex data.
    /// Apart from that, the class binds any provided index
    /// buffer attached to the vertex data.
    class VertexArray
    {
        public:

            uint32_t vtxArrID;

            /// @brief Creates an OpenGL vertex array.
            VertexArray();

            /// @brief Creates an OpenGL vertex array from a vertex
            ///     buffer and an index buffer.
            VertexArray(const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib);

            void bind() const;
            void unbind() const;

            void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
            void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer);
            const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const { return vertexBuffers; }
            const Ref<IndexBuffer>& getIndexBuffer() const { return indexBuffer; }

        private:

            std::vector<Ref<VertexBuffer>> vertexBuffers;
            Ref<IndexBuffer> indexBuffer;
    };
}