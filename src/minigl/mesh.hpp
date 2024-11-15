#pragma once

#include "core.hpp"
#include "buffer.hpp"

namespace minigl
{
    /// A mesh is a collection of vertices and indices put
    /// together in a vertex array.
    struct Mesh
    {
        Ref<VertexArray> vertexArray;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Mesh() = default;

        /// Construct a mesh from a set of vertices and
        /// indices, with a certain usage (`StaticDraw` by default;
        /// see `DataUsage`). Internally, a vertex array is
        /// created, but not bound.
        Mesh(const std::vector<Vertex>& vertices, 
            const std::vector<uint32_t>& indices,
            DataUsage usage = DataUsage::Static);

        /// Load a mesh from an OBJ file at the given path.
        Mesh(const std::string& path, DataUsage usage = DataUsage::Static);
    };
}