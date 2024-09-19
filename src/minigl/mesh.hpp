#pragma once

#include "core.hpp"
#include "buffer.hpp"

namespace minigl
{
    /// @brief Helper class: a model is formed of a mesh or
    /// collection of meshes.
    class Mesh
    {
        public:

            Mesh() = default;

            /// @brief A mesh is constructed from the vertices,
            /// indices and textures
            ///
            /// @details A vertex array is created from the
            /// mesh vertices and indices, but not bound.
            Mesh(const std::vector<Vertex>& vertices, 
                const std::vector<uint32_t>& indices,
                DataUsage usage = DataUsage::Static);

            /// @brief A mesh constructed from an OBJ file.
            Mesh(const std::string& filepath, DataUsage usage = DataUsage::Static);

        public:

            Ref<VertexArray> vertexArray;
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
    };
}